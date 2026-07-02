/* df.c — report filesystem disk space usage. */
#include <stdio.h>
#include <sys/statvfs.h>
#include "utilbox.h"

int df_main(int argc, char **argv)
{
    const char *path = (argc >= 2) ? argv[1] : "/";

    struct statvfs vfs;
    if (statvfs(path, &vfs) != 0) {
        return die("df", "cannot stat filesystem", path);
    }

    unsigned long block_size = vfs.f_frsize ? vfs.f_frsize : vfs.f_bsize;
    unsigned long total_kb = (vfs.f_blocks * block_size) / 1024;
    unsigned long free_kb  = (vfs.f_bfree  * block_size) / 1024;
    unsigned long used_kb  = total_kb - free_kb;

    printf("%-20s %10s %10s %10s %s\n", "Filesystem", "1K-blocks", "Used", "Available", "Mounted on");
    printf("%-20s %10lu %10lu %10lu %s\n", "-", total_kb, used_kb, free_kb, path);

    return 0;
}
