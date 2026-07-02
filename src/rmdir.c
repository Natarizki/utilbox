/* rmdir.c — remove empty directories */
#include <unistd.h>
#include "utilbox.h"

int rmdir_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("rmdir", "missing operand");
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        if (rmdir(argv[i]) != 0) {
            rc |= die("rmdir", "failed to remove", argv[i]);
        }
    }

    return rc;
}
