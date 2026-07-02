/* stat.c — display detailed file status */
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include "utilbox.h"

int stat_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("stat", "usage: stat FILE...");
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        struct stat st;
        if (lstat(argv[i], &st) != 0) {
            rc |= die("stat", "cannot stat", argv[i]);
            continue;
        }

        char mtimebuf[64];
        strftime(mtimebuf, sizeof(mtimebuf), "%Y-%m-%d %H:%M:%S", localtime(&st.st_mtime));

        printf("  File: %s\n", argv[i]);
        printf("  Size: %-10ld Blocks: %-10ld IO Block: %-6ld\n",
                (long)st.st_size, (long)st.st_blocks, (long)st.st_blksize);
        printf("Device: %ld  Inode: %lu  Links: %lu\n",
                (long)st.st_dev, (unsigned long)st.st_ino, (unsigned long)st.st_nlink);
        printf("Access: (%04o)  Uid: %d  Gid: %d\n",
                st.st_mode & 07777, st.st_uid, st.st_gid);
        printf("Modify: %s\n\n", mtimebuf);
    }

    return rc;
}
