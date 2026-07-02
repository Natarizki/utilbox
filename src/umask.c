/* umask.c — print or set the file mode creation mask */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "utilbox.h"

int umask_main(int argc, char **argv)
{
    if (argc >= 2) {
        mode_t m = (mode_t)strtol(argv[1], NULL, 8);
        umask(m);
        return 0;
    }

    mode_t current = umask(0);
    umask(current);
    printf("%04o\n", current);
    return 0;
}
