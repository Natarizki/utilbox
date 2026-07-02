/* xargs.c — read whitespace-separated tokens from stdin, append them to the
 * given command, and execute it. Simplified: one exec with all args batched
 * together (no -n batching, no -0 null-separated support in v1). */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utilbox.h"

#define MAX_ARGS 256

int xargs_main(int argc, char **argv)
{
    const char *cmd_args[MAX_ARGS];
    int ncmd = 0;

    if (argc < 2) {
        cmd_args[ncmd++] = "echo";
    } else {
        for (int i = 1; i < argc && ncmd < MAX_ARGS; i++) {
            cmd_args[ncmd++] = argv[i];
        }
    }

    char token[4096];
    int c;
    size_t tlen = 0;

    while (ncmd < MAX_ARGS - 1) {
        c = fgetc(stdin);
        if (c == EOF || c == ' ' || c == '\t' || c == '\n') {
            if (tlen > 0) {
                token[tlen] = '\0';
                cmd_args[ncmd++] = strdup(token);
                tlen = 0;
            }
            if (c == EOF) break;
        } else {
            if (tlen < sizeof(token) - 1) {
                token[tlen++] = (char)c;
            }
        }
    }

    cmd_args[ncmd] = NULL;

    if (ncmd == 0 || (ncmd == 1 && argc < 2)) {
        return 0;
    }

    pid_t pid = fork();
    if (pid < 0) {
        return die("xargs", "fork failed", NULL);
    }

    if (pid == 0) {
        execvp(cmd_args[0], (char **)cmd_args);
        fprintf(stderr, "utilbox: xargs: %s: command not found\n", cmd_args[0]);
        _exit(127);
    }

    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
