/* mkfifo.c — create named pipes (FIFOs) */
#include <sys/stat.h>
#include "utilbox.h"

int mkfifo_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("mkfifo", "usage: mkfifo PATH...");
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        if (mkfifo(argv[i], 0666) != 0) {
            rc |= die("mkfifo", "cannot create", argv[i]);
        }
    }

    return rc;
}
