/* mknod.c — create a special file (character or block device, or fifo).
 * Usage: mknod PATH TYPE [MAJOR MINOR]   TYPE: c (char), b (block), p (fifo) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include "utilbox.h"

int mknod_main(int argc, char **argv)
{
    if (argc < 3) {
        return fail("mknod", "usage: mknod PATH TYPE [MAJOR MINOR]");
    }

    const char *path = argv[1];
    char type = argv[2][0];

    mode_t mode = 0666;
    dev_t dev = 0;

    if (type == 'p') {
        mode |= S_IFIFO;
    }
    else if (type == 'c' || type == 'b') {
        if (argc < 5) {
            return fail("mknod", "device types require MAJOR and MINOR");
        }
        int major_n = atoi(argv[3]);
        int minor_n = atoi(argv[4]);
        dev = makedev(major_n, minor_n);
        mode |= (type == 'c') ? S_IFCHR : S_IFBLK;
    }
    else {
        return fail("mknod", "unknown type (use c, b, or p)");
    }

    if (mknod(path, mode, dev) != 0) {
        return die("mknod", "cannot create", path);
    }

    return 0;
}
