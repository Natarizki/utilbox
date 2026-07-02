/* watch.c — run a command repeatedly, clearing the screen between runs.
 * Usage: watch [-n SECONDS] COMMAND [ARGS...]  (default interval: 2s) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utilbox.h"

int watch_main(int argc, char **argv)
{
    int interval = 2;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
        interval = atoi(argv[i + 1]);
        i += 2;
    }

    if (i >= argc) {
        return fail("watch", "usage: watch [-n SECONDS] COMMAND [ARGS...]");
    }

    for (;;) {
        fputs("\033[H\033[2J", stdout);
        fflush(stdout);

        pid_t pid = fork();
        if (pid == 0) {
            execvp(argv[i], argv + i);
            fprintf(stderr, "utilbox: watch: %s: command not found\n", argv[i]);
            _exit(127);
        }

        int status;
        waitpid(pid, &status, 0);
        sleep(interval);
    }

    return 0;
}
