/* lsblk.c — list block devices, reading names and sizes from /sys/block */
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "utilbox.h"

int lsblk_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    DIR *d = opendir("/sys/block");
    if (!d) return die("lsblk", "cannot open /sys/block", NULL);

    printf("%-10s %10s\n", "NAME", "SIZE(blocks)");

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;

        char path[256];
        snprintf(path, sizeof(path), "/sys/block/%s/size", ent->d_name);
        FILE *f = fopen(path, "r");
        long size = 0;
        if (f) {
            fscanf(f, "%ld", &size);
            fclose(f);
        }

        printf("%-10s %10ld\n", ent->d_name, size);
    }

    closedir(d);
    return 0;
}
