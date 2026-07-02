/* truncate.c — shrink or extend a file to a specified size.
 * Usage: truncate -s SIZE FILE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utilbox.h"

int truncate_main(int argc, char **argv)
{
    if (argc != 4 || strcmp(argv[1], "-s") != 0) {
        return fail("truncate", "usage: truncate -s SIZE FILE");
    }

    off_t size = (off_t)atol(argv[2]);
    const char *path = argv[3];

    if (truncate(path, size) != 0) {
        return die("truncate", "cannot truncate", path);
    }

    return 0;
}
