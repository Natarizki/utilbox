/* kill.c — send a signal to a process. Usage: kill [-SIGNUM] PID... */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "utilbox.h"

int kill_main(int argc, char **argv)
{
    int sig = SIGTERM;
    int i = 1;

    if (i < argc && argv[i][0] == '-') {
        sig = atoi(argv[i] + 1);
        if (sig == 0) sig = SIGTERM;
        i++;
    }

    if (i >= argc) {
        return fail("kill", "usage: kill [-SIGNUM] PID...");
    }

    int rc = 0;
    for (; i < argc; i++) {
        pid_t pid = (pid_t)atol(argv[i]);
        if (kill(pid, sig) != 0) {
            rc |= die("kill", "cannot send signal to", argv[i]);
        }
    }

    return rc;
}
