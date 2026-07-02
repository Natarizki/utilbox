/* fsync.c — flush a file's buffered data to disk */
#include <fcntl.h>
#include <unistd.h>
#include "utilbox.h"

int fsync_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("fsync", "usage: fsync FILE...");
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            rc |= die("fsync", "cannot open", argv[i]);
            continue;
        }
        if (fsync(fd) != 0) {
            rc |= die("fsync", "cannot sync", argv[i]);
        }
        close(fd);
    }

    return rc;
}
