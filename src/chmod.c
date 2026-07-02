/* chmod.c — change file mode (permissions).
 * Only supports octal mode (e.g. 755, 0644), not symbolic (u+x, etc) in v1. */
#include <sys/stat.h>
#include <stdlib.h>
#include "utilbox.h"

int chmod_main(int argc, char **argv)
{
    if (argc < 3) {
        return fail("chmod", "usage: chmod MODE FILE...");
    }

    mode_t mode = (mode_t)strtol(argv[1], NULL, 8);

    int rc = 0;
    for (int i = 2; i < argc; i++) {
        if (chmod(argv[i], mode) != 0) {
            rc |= die("chmod", "cannot change permissions of", argv[i]);
        }
    }

    return rc;
}
