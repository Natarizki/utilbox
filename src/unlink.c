/* unlink.c — remove a single file (raw unlink() syscall wrapper) */
#include <unistd.h>
#include "utilbox.h"

int unlink_main(int argc, char **argv)
{
    if (argc != 2) {
        return fail("unlink", "usage: unlink FILE");
    }

    if (unlink(argv[1]) != 0) {
        return die("unlink", "cannot unlink", argv[1]);
    }

    return 0;
}
