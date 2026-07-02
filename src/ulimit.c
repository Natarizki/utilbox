/* ulimit.c — print or set resource limits.
 * Usage: ulimit -n [VALUE]  (open files)   ulimit -f [VALUE]  (file size, KB) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include "utilbox.h"

int ulimit_main(int argc, char **argv)
{
    int resource = RLIMIT_NOFILE;

    if (argc >= 2) {
        if (strcmp(argv[1], "-n") == 0) resource = RLIMIT_NOFILE;
        else if (strcmp(argv[1], "-f") == 0) resource = RLIMIT_FSIZE;
        else if (strcmp(argv[1], "-c") == 0) resource = RLIMIT_CORE;
        else if (strcmp(argv[1], "-u") == 0) resource = RLIMIT_NPROC;
        else return fail("ulimit", "usage: ulimit [-n|-f|-c|-u] [VALUE]");
    }

    struct rlimit rl;
    getrlimit(resource, &rl);

    if (argc >= 3) {
        rl.rlim_cur = (rlim_t)atol(argv[2]);
        if (setrlimit(resource, &rl) != 0) {
            return die("ulimit", "cannot set limit", NULL);
        }
        return 0;
    }

    if (rl.rlim_cur == RLIM_INFINITY) {
        printf("unlimited\n");
    } else {
        printf("%lu\n", (unsigned long)rl.rlim_cur);
    }

    return 0;
}
