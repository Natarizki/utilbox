/* ionice.c — run a command with a given I/O scheduling class/priority.
 * Usage: ionice -c CLASS -n PRIO COMMAND [ARGS...]
 * CLASS: 1=realtime, 2=best-effort, 3=idle */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "utilbox.h"

#define IOPRIO_WHO_PROCESS 1
#define IOPRIO_CLASS_SHIFT 13

static int ioprio_set(int which, int who, int ioprio)
{
    return syscall(SYS_ioprio_set, which, who, ioprio);
}

int ionice_main(int argc, char **argv)
{
    int class = 2;
    int prio = 4;
    int i = 1;

    while (i < argc) {
        if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            class = atoi(argv[i + 1]);
            i += 2;
        }
        else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            prio = atoi(argv[i + 1]);
            i += 2;
        }
        else {
            break;
        }
    }

    if (i >= argc) {
        return fail("ionice", "usage: ionice -c CLASS -n PRIO COMMAND [ARGS...]");
    }

    int ioprio_value = (class << IOPRIO_CLASS_SHIFT) | prio;
    if (ioprio_set(IOPRIO_WHO_PROCESS, 0, ioprio_value) != 0) {
        fprintf(stderr, "utilbox: ionice: warning: cannot set I/O priority\n");
    }

    execvp(argv[i], argv + i);
    return die("ionice", "cannot execute", argv[i]);
}
