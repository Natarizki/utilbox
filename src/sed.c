/* sed.c — simplified stream editor. Supports s/PATTERN/REPL/[g] using
 * POSIX regex, and line deletion via /PATTERN/d. */
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "utilbox.h"

static void apply_subst(char *line, regex_t *re, const char *repl, int global)
{
    char result[65536];
    result[0] = '\0';
    char *p = line;
    regmatch_t m;
    int did_one = 0;

    while (*p) {
        if ((did_one && !global) || regexec(re, p, 1, &m, 0) != 0) {
            strcat(result, p);
            break;
        }
        strncat(result, p, m.rm_so);
        strcat(result, repl);
        p += m.rm_eo;
        did_one = 1;
        if (m.rm_so == m.rm_eo) {
            if (*p) strncat(result, p, 1);
            else break;
            p++;
        }
        if (!global) {
            strcat(result, p);
            break;
        }
    }

    strcpy(line, result);
}

int sed_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("sed", "usage: sed 'SCRIPT' [FILE]");
    }

    const char *script = argv[1];
    FILE *in = (argc >= 3) ? fopen(argv[2], "r") : stdin;
    if (!in) return die("sed", "cannot open", argv[2]);

    char line[65536];

    if (script[0] == 's' && (script[1] == '/' || script[1] == '|')) {
        char delim = script[1];
        char pattern[512], repl[512];
        int global = 0;

        const char *p = script + 2;
        const char *end1 = strchr(p, delim);
        if (!end1) return fail("sed", "malformed s/// script");
        size_t plen = end1 - p;
        strncpy(pattern, p, plen); pattern[plen] = '\0';

        p = end1 + 1;
        const char *end2 = strchr(p, delim);
        if (!end2) return fail("sed", "malformed s/// script");
        size_t rlen = end2 - p;
        strncpy(repl, p, rlen); repl[rlen] = '\0';

        if (strchr(end2 + 1, 'g')) global = 1;

        regex_t re;
        if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
            return fail("sed", "invalid regex pattern");
        }

        while (fgets(line, sizeof(line), in) != NULL) {
            size_t len = strlen(line);
            int had_nl = (len > 0 && line[len-1] == '\n');
            if (had_nl) line[len-1] = '\0';
            apply_subst(line, &re, repl, global);
            fputs(line, stdout);
            if (had_nl) putchar('\n');
        }
        regfree(&re);
    }
    else if (script[0] == '/' ) {
        const char *end = strrchr(script, '/');
        char pattern[512];
        size_t plen = end - (script + 1);
        strncpy(pattern, script + 1, plen); pattern[plen] = '\0';

        regex_t re;
        if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
            return fail("sed", "invalid regex pattern");
        }

        while (fgets(line, sizeof(line), in) != NULL) {
            if (regexec(&re, line, 0, NULL, 0) != 0) {
                fputs(line, stdout);
            }
        }
        regfree(&re);
    }
    else {
        return fail("sed", "unsupported script (only s/// and /pat/d supported)");
    }

    if (in != stdin) fclose(in);
    return 0;
}
