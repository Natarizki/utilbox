/* wall.c — broadcast a message to all terminals under /dev/pts */
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "utilbox.h"

int wall_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("wall", "usage: wall MESSAGE");
    }

    DIR *d = opendir("/dev/pts");
    if (!d) return die("wall", "cannot open /dev/pts", NULL);

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;

        char path[64];
        snprintf(path, sizeof(path), "/dev/pts/%s", ent->d_name);
        FILE *f = fopen(path, "w");
        if (f) {
            fprintf(f, "\nBroadcast message: %s\n", argv[1]);
            fclose(f);
        }
    }

    closedir(d);
    return 0;
}
