/* nice.c — run a command with modified scheduling priority.
 * Usage: nice [-n ADJUSTMENT] COMMAND [ARGS...]  (default adjustment: 10) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utilbox.h"

int nice_main(int argc, char **argv)
{
    int adj = 10;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
        adj = atoi(argv[i + 1]);
        i += 2;
    }

    if (i >= argc) {
        return fail("nice", "usage: nice [-n ADJUSTMENT] COMMAND [ARGS...]");
    }

    if (nice(adj) == -1) {
        fprintf(stderr, "utilbox: nice: warning: cannot set niceness\n");
    }

    execvp(argv[i], argv + i);
    return die("nice", "cannot execute", argv[i]);
}
