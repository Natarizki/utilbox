/* shred.c — overwrite a file's contents before removal, to make recovery
 * harder. Overwrites with zero bytes 3 times by default. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "utilbox.h"

int shred_main(int argc, char **argv)
{
    int remove_after = 0;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-u") == 0) {
        remove_after = 1;
        i++;
    }

    if (i >= argc) {
        return fail("shred", "usage: shred [-u] FILE...");
    }

    int rc = 0;
    for (; i < argc; i++) {
        struct stat st;
        if (stat(argv[i], &st) != 0) {
            rc |= die("shred", "cannot stat", argv[i]);
            continue;
        }

        int fd = open(argv[i], O_WRONLY);
        if (fd < 0) {
            rc |= die("shred", "cannot open", argv[i]);
            continue;
        }

        char zeros[4096];
        memset(zeros, 0, sizeof(zeros));

        for (int pass = 0; pass < 3; pass++) {
            lseek(fd, 0, SEEK_SET);
            off_t remaining = st.st_size;
            while (remaining > 0) {
                size_t chunk = (remaining < (off_t)sizeof(zeros)) ? (size_t)remaining : sizeof(zeros);
                write(fd, zeros, chunk);
                remaining -= chunk;
            }
        }

        close(fd);

        if (remove_after) {
            unlink(argv[i]);
        }
    }

    return rc;
}
