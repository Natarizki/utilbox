/* sh.c — utilbox shell. Supports:
 *   - pipelines: cmd1 | cmd2 | cmd3
 *   - redirection: cmd > file, cmd >> file, cmd < file
 *   - variables: $VAR / ${VAR} expansion, NAME=VALUE assignment, export NAME=VALUE
 *   - interactive REPL mode (prompt shown when stdin is a tty)
 *   - builtins: cd, exit
 *   - typo auto-correction: if a command isn't found, suggests/runs the
 *     closest known command (built-in applet or $PATH executable) based on
 *     Levenshtein edit distance.
 *
 * Usage: sh script.sh
 *        sh -c "command args"
 *        sh              (interactive REPL if stdin is a tty, else reads
 *                         commands from stdin non-interactively)
 *
 * Limitations (v1): no quoting/escaping awareness (redirection and pipe
 * characters inside quoted strings are not protected), no && / || / ;
 * chaining, no background jobs (&), no command substitution.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include "utilbox.h"
#include "applets.h"

#define MAX_ARGS 64
#define MAX_LINE 4096
#define MAX_CANDIDATES 4096
#define MAX_STAGES 16

/* ---------------- typo auto-correction ---------------- */

static int levenshtein(const char *a, const char *b)
{
    int la = (int)strlen(a), lb = (int)strlen(b);
    int *prev = malloc((lb + 1) * sizeof(int));
    int *curr = malloc((lb + 1) * sizeof(int));

    for (int j = 0; j <= lb; j++) prev[j] = j;

    for (int i = 1; i <= la; i++) {
        curr[0] = i;
        for (int j = 1; j <= lb; j++) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            int del = prev[j] + 1;
            int ins = curr[j - 1] + 1;
            int sub = prev[j - 1] + cost;
            int m = (del < ins) ? del : ins;
            if (sub < m) m = sub;
            curr[j] = m;
        }
        int *tmp = prev; prev = curr; curr = tmp;
    }

    int result = prev[lb];
    free(prev);
    free(curr);
    return result;
}

static int collect_candidates(char **out, int maxout)
{
    int n = 0;

    for (int i = 0; applets[i].name != NULL && n < maxout; i++) {
        out[n++] = (char *)applets[i].name;
    }

    const char *path_env = getenv("PATH");
    if (!path_env) path_env = "/usr/bin:/bin";

    char pathbuf[4096];
    strncpy(pathbuf, path_env, sizeof(pathbuf) - 1);
    pathbuf[sizeof(pathbuf) - 1] = '\0';

    char *saveptr;
    char *dir = strtok_r(pathbuf, ":", &saveptr);
    while (dir != NULL && n < maxout) {
        DIR *d = opendir(dir);
        if (d) {
            struct dirent *ent;
            while ((ent = readdir(d)) != NULL && n < maxout) {
                if (ent->d_name[0] == '.') continue;
                out[n++] = strdup(ent->d_name);
            }
            closedir(d);
        }
        dir = strtok_r(NULL, ":", &saveptr);
    }

    return n;
}

static char *find_correction(const char *typed)
{
    static char *candidates[MAX_CANDIDATES];
    static int ncandidates = -1;

    if (ncandidates < 0) {
        ncandidates = collect_candidates(candidates, MAX_CANDIDATES);
    }

    int typed_len = (int)strlen(typed);
    int threshold = (typed_len <= 3) ? 1 : 2;

    char *best = NULL;
    int best_dist = threshold + 1;

    for (int i = 0; i < ncandidates; i++) {
        if (candidates[i][0] != typed[0]) continue;
        if (strcmp(candidates[i], typed) == 0) continue;

        int dist = levenshtein(typed, candidates[i]);
        if (dist <= threshold && dist < best_dist) {
            best_dist = dist;
            best = candidates[i];
        }
    }

    return best ? strdup(best) : NULL;
}

/* ---------------- variable expansion ---------------- */

static void expand_variables(const char *in, char *out, size_t outsz)
{
    size_t oi = 0;
    for (size_t i = 0; in[i] && oi < outsz - 1; ) {
        int at_word_start = (i == 0) || (in[i - 1] == ' ') || (in[i - 1] == '\t');

        if (in[i] == '~' && at_word_start &&
                (in[i + 1] == '/' || in[i + 1] == '\0' || in[i + 1] == ' ' || in[i + 1] == '\t')) {
            const char *home = getenv("HOME");
            if (home) {
                size_t hlen = strlen(home);
                if (oi + hlen < outsz - 1) {
                    memcpy(out + oi, home, hlen);
                    oi += hlen;
                }
            }
            i++;
        }
        else if (in[i] == '$' && (isalpha((unsigned char)in[i + 1]) || in[i + 1] == '_' || in[i + 1] == '{')) {
            i++;
            int braced = 0;
            if (in[i] == '{') { braced = 1; i++; }

            char name[256];
            int ni = 0;
            while (in[i] && (isalnum((unsigned char)in[i]) || in[i] == '_') && ni < 255) {
                name[ni++] = in[i++];
            }
            name[ni] = '\0';
            if (braced && in[i] == '}') i++;

            const char *val = getenv(name);
            if (val) {
                size_t vlen = strlen(val);
                if (oi + vlen < outsz - 1) {
                    memcpy(out + oi, val, vlen);
                    oi += vlen;
                }
            }
        } else {
            out[oi++] = in[i++];
        }
    }
    out[oi] = '\0';
}

static int is_name_char(char c, int first)
{
    if (isalpha((unsigned char)c) || c == '_') return 1;
    if (!first && isdigit((unsigned char)c)) return 1;
    return 0;
}

static int try_builtin_assignment(const char *line)
{
    const char *p = line;
    if (strncmp(p, "export ", 7) == 0) {
        p += 7;
        while (*p == ' ') p++;
    }

    const char *eq = strchr(p, '=');
    if (!eq || eq == p) return 0;

    for (const char *c = p; c < eq; c++) {
        if (!is_name_char(*c, c == p)) return 0;
    }

    size_t namelen = eq - p;
    char name[256];
    if (namelen >= sizeof(name)) return 0;
    memcpy(name, p, namelen);
    name[namelen] = '\0';

    char expanded[MAX_LINE];
    expand_variables(eq + 1, expanded, sizeof(expanded));

    setenv(name, expanded, 1);
    return 1;
}

/* ---------------- pipeline execution ---------------- */

typedef struct {
    char *args[MAX_ARGS];
    int nargs;
    char *infile;
    char *outfile;
    int append;
} stage_t;

static int run_pipeline(char *line)
{
    stage_t stages[MAX_STAGES];
    int nstages = 0;

    char *saveptr1, *saveptr2;
    char *stage_text = strtok_r(line, "|", &saveptr1);

    while (stage_text && nstages < MAX_STAGES) {
        stage_t *st = &stages[nstages];
        st->nargs = 0;
        st->infile = NULL;
        st->outfile = NULL;
        st->append = 0;

        char *tok = strtok_r(stage_text, " \t\n", &saveptr2);
        while (tok != NULL) {
            if (strcmp(tok, ">") == 0) {
                tok = strtok_r(NULL, " \t\n", &saveptr2);
                if (tok) { st->outfile = tok; st->append = 0; }
            }
            else if (strcmp(tok, ">>") == 0) {
                tok = strtok_r(NULL, " \t\n", &saveptr2);
                if (tok) { st->outfile = tok; st->append = 1; }
            }
            else if (strcmp(tok, "<") == 0) {
                tok = strtok_r(NULL, " \t\n", &saveptr2);
                if (tok) st->infile = tok;
            }
            else if (st->nargs < MAX_ARGS - 1) {
                st->args[st->nargs++] = tok;
            }

            if (tok) tok = strtok_r(NULL, " \t\n", &saveptr2);
        }
        st->args[st->nargs] = NULL;

        if (st->nargs > 0) nstages++;
        stage_text = strtok_r(NULL, "|", &saveptr1);
    }

    if (nstages == 0) return 0;
    if (stages[0].args[0][0] == '#') return 0;

    int pipes[MAX_STAGES - 1][2];
    for (int i = 0; i < nstages - 1; i++) {
        if (pipe(pipes[i]) != 0) {
            return die("sh", "pipe() failed", NULL);
        }
    }

    pid_t pids[MAX_STAGES];

    for (int i = 0; i < nstages; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            return die("sh", "fork failed", NULL);
        }

        if (pid == 0) {
            if (i > 0) dup2(pipes[i - 1][0], 0);
            if (i < nstages - 1) dup2(pipes[i][1], 1);

            for (int j = 0; j < nstages - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            if (stages[i].infile) {
                int fd = open(stages[i].infile, O_RDONLY);
                if (fd < 0) { die("sh", "cannot open", stages[i].infile); _exit(1); }
                dup2(fd, 0);
                close(fd);
            }
            if (stages[i].outfile) {
                int flags = O_WRONLY | O_CREAT | (stages[i].append ? O_APPEND : O_TRUNC);
                int fd = open(stages[i].outfile, flags, 0666);
                if (fd < 0) { die("sh", "cannot open", stages[i].outfile); _exit(1); }
                dup2(fd, 1);
                close(fd);
            }

            execvp(stages[i].args[0], stages[i].args);

            char *corrected = find_correction(stages[i].args[0]);
            if (corrected) {
                fprintf(stderr, "utilbox: sh: '%s' not found, did you mean '%s'? running it...\n",
                        stages[i].args[0], corrected);
                stages[i].args[0] = corrected;
                execvp(stages[i].args[0], stages[i].args);
            }

            fprintf(stderr, "utilbox: sh: %s: command not found\n", stages[i].args[0]);
            _exit(127);
        }

        pids[i] = pid;
    }

    for (int j = 0; j < nstages - 1; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

    int status = 0;
    for (int i = 0; i < nstages; i++) {
        int st;
        waitpid(pids[i], &st, 0);
        if (i == nstages - 1) status = st;
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

/* ---------------- line dispatch (builtins + pipeline) ---------------- */

static int run_segment(char *segment)
{
    char *trimmed = segment;
    while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
    if (*trimmed == '\0' || *trimmed == '#') return 0;

    if (try_builtin_assignment(trimmed)) {
        return 0;
    }

    char expanded[MAX_LINE];
    expand_variables(trimmed, expanded, sizeof(expanded));

    if (strcmp(expanded, "cd") == 0 || strncmp(expanded, "cd ", 3) == 0) {
        char *arg = expanded + 2;
        while (*arg == ' ') arg++;
        const char *target = (*arg) ? arg : getenv("HOME");
        if (!target) target = "/";
        if (chdir(target) != 0) {
            return die("sh", "cd: cannot change directory to", target);
        }
        return 0;
    }

    if (strcmp(expanded, "exit") == 0 || strncmp(expanded, "exit ", 5) == 0) {
        int code = (strlen(expanded) > 5) ? atoi(expanded + 5) : 0;
        exit(code);
    }

    return run_pipeline(expanded);
}

static int run_line(char *line)
{
    char *trimmed = line;
    while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
    if (*trimmed == '\0' || *trimmed == '\n' || *trimmed == '#') return 0;

    char linecopy[MAX_LINE];
    strncpy(linecopy, trimmed, sizeof(linecopy) - 1);
    linecopy[sizeof(linecopy) - 1] = '\0';
    linecopy[strcspn(linecopy, "\n")] = '\0';

    int rc = 0;
    char *saveptr;
    char *seg = strtok_r(linecopy, ";", &saveptr);
    while (seg != NULL) {
        rc = run_segment(seg);
        seg = strtok_r(NULL, ";", &saveptr);
    }
    return rc;
}

static int run_stream(FILE *in)
{
    char line[MAX_LINE];
    int rc = 0;
    int interactive = isatty(fileno(in));

    if (interactive) {
        printf("$ ");
        fflush(stdout);
    }

    while (fgets(line, sizeof(line), in) != NULL) {
        rc = run_line(line);
        if (interactive) {
            printf("$ ");
            fflush(stdout);
        }
    }

    if (interactive) printf("\n");
    return rc;
}

int sh_main(int argc, char **argv)
{
    if (argc >= 3 && strcmp(argv[1], "-c") == 0) {
        char cmdbuf[MAX_LINE];
        strncpy(cmdbuf, argv[2], sizeof(cmdbuf) - 1);
        cmdbuf[sizeof(cmdbuf) - 1] = '\0';
        return run_line(cmdbuf);
    }

    if (argc >= 2) {
        FILE *f = fopen(argv[1], "r");
        if (!f) return die("sh", "cannot open script", argv[1]);
        int rc = run_stream(f);
        fclose(f);
        return rc;
    }

    return run_stream(stdin);
}
