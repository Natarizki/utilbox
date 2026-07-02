/* chgrp.c — change file group ownership */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <grp.h>
#include "utilbox.h"

int chgrp_main(int argc, char **argv)
{
    if (argc < 3) {
        return fail("chgrp", "usage: chgrp GROUP FILE...");
    }

    struct group *gr = getgrnam(argv[1]);
    gid_t gid = gr ? gr->gr_gid : (gid_t)atoi(argv[1]);

    int rc = 0;
    for (int i = 2; i < argc; i++) {
        if (chown(argv[i], (uid_t)-1, gid) != 0) {
            rc |= die("chgrp", "cannot change group of", argv[i]);
        }
    }

    return rc;
}
