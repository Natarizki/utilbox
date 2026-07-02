/* mv.c — move/rename files. Falls back to copy+delete if rename() fails
 * across filesystems (EXDEV). */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "utilbox.h"

extern int cp_main(int argc, char **argv);
extern int rm_main(int argc, char **argv);

int mv_main(int argc, char **argv)
{
    if (argc < 3) {
        return fail("mv", "usage: mv SOURCE... DEST");
    }

    const char *dst = argv[argc - 1];
    int rc = 0;

    for (int i = 1; i < argc - 1; i++) {
        if (rename(argv[i], dst) == 0) {
            continue;
        }

        if (errno == EXDEV) {
            /* Cross-filesystem: copy then remove original. */
            struct stat st;
            const char *cp_args[4] = { "cp", "", "", NULL };
            int cp_argc;
            if (stat(argv[i], &st) == 0 && S_ISDIR(st.st_mode)) {
                const char *cp_r_args[5] = { "cp", "-r", argv[i], dst, NULL };
                cp_argc = 4;
                rc |= cp_main(cp_argc, (char **)cp_r_args);
            } else {
                cp_args[1] = argv[i];
                cp_args[2] = dst;
                cp_argc = 3;
                rc |= cp_main(cp_argc, (char **)cp_args);
            }

            const char *rm_args[4] = { "rm", "-rf", argv[i], NULL };
            rc |= rm_main(3, (char **)rm_args);
        }
        else {
            rc |= die("mv", "cannot move", argv[i]);
        }
    }

    return rc;
}
