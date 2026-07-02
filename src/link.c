/* link.c — create a hard link (raw link() syscall wrapper, no -s option
 * unlike ln — this is the simpler POSIX `link` utility). */
#include <unistd.h>
#include "utilbox.h"

int link_main(int argc, char **argv)
{
    if (argc != 3) {
        return fail("link", "usage: link TARGET LINKNAME");
    }

    if (link(argv[1], argv[2]) != 0) {
        return die("link", "cannot create link", argv[2]);
    }

    return 0;
}
