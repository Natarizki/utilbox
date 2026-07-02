/* unshare.c — run a command in a new namespace.
 * Usage: unshare [-m] [-u] [-p] [-n] COMMAND [ARGS...]
 * -m mount, -u uts (hostname), -p pid, -n network */
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include "utilbox.h"

int unshare_main(int argc, char **argv)
{
    int flags = 0;
    int i = 1;

    for (; i < argc && argv[i][0] == '-'; i++) {
        if (strchr(argv[i], 'm')) flags |= CLONE_NEWNS;
        if (strchr(argv[i], 'u')) flags |= CLONE_NEWUTS;
        if (strchr(argv[i], 'p')) flags |= CLONE_NEWPID;
        if (strchr(argv[i], 'n')) flags |= CLONE_NEWNET;
    }

    if (i >= argc) {
        return fail("unshare", "usage: unshare [-munp] COMMAND [ARGS...]");
    }

    if (unshare(flags) != 0) {
        return die("unshare", "cannot unshare namespace (need privilege)", NULL);
    }

    execvp(argv[i], argv + i);
    return die("unshare", "cannot execute", argv[i]);
}
