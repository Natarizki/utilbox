/* realpath.c — resolve a path to its canonical absolute form */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "utilbox.h"

int realpath_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("realpath", "usage: realpath PATH");
    }

    char buf[PATH_MAX];
    if (realpath(argv[1], buf) == NULL) {
        return die("realpath", "cannot resolve", argv[1]);
    }

    puts(buf);
    return 0;
}
