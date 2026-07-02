/* ln.c — create links. Supports -s (symbolic link); default is hard link. */
#include <unistd.h>
#include <string.h>
#include "utilbox.h"

int ln_main(int argc, char **argv)
{
    int symbolic = 0;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-s") == 0) {
        symbolic = 1;
        i++;
    }

    if (i + 2 > argc) {
        return fail("ln", "usage: ln [-s] TARGET LINK_NAME");
    }

    const char *target = argv[i];
    const char *linkname = argv[i + 1];

    int rc;
    if (symbolic) {
        rc = symlink(target, linkname);
    } else {
        rc = link(target, linkname);
    }

    if (rc != 0) {
        return die("ln", "cannot create link", linkname);
    }

    return 0;
}
