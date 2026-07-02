/* timeout.c — run a command, killing it if it exceeds the given duration.
 * Usage: timeout SECONDS COMMAND [ARGS...] */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "utilbox.h"

int timeout_main(int argc, char **argv)
{
    if (argc < 3) {
        return fail("timeout", "usage: timeout SECONDS COMMAND [ARGS...]");
    }

    int secs = atoi(argv[1]);

    pid_t pid = fork();
    if (pid < 0) {
        return die("timeout", "fork failed", NULL);
    }

    if (pid == 0) {
        execvp(argv[2], argv + 2);
        fprintf(stderr, "utilbox: timeout: %s: command not found\n", argv[2]);
        _exit(127);
    }

    pid_t timer_pid = fork();
    if (timer_pid == 0) {
        sleep(secs);
        kill(pid, SIGTERM);
        _exit(0);
    }

    int status;
    waitpid(pid, &status, 0);
    kill(timer_pid, SIGKILL);
    waitpid(timer_pid, NULL, 0);

    return WIFEXITED(status) ? WEXITSTATUS(status) : 124;
}
