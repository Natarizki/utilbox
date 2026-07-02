/* renice.c — change the scheduling priority of a running process.
 * Usage: renice PRIORITY PID */
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include "utilbox.h"

int renice_main(int argc, char **argv)
{
    if (argc != 3) {
        return fail("renice", "usage: renice PRIORITY PID");
    }

    int prio = atoi(argv[1]);
    pid_t pid = (pid_t)atol(argv[2]);

    if (setpriority(PRIO_PROCESS, pid, prio) != 0) {
        return die("renice", "cannot set priority for", argv[2]);
    }

    return 0;
}
