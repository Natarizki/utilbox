/* pidof.c — find the PID(s) of a running process by name, via /proc */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "utilbox.h"

int pidof_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("pidof", "usage: pidof NAME");
    }

    DIR *d = opendir("/proc");
    if (!d) return die("pidof", "cannot open /proc", NULL);

    int found = 0;
    struct dirent *ent;

    while ((ent = readdir(d)) != NULL) {
        if (!isdigit((unsigned char)ent->d_name[0])) continue;

        char path[64];
        snprintf(path, sizeof(path), "/proc/%s/comm", ent->d_name);
        FILE *f = fopen(path, "r");
        if (!f) continue;

        char comm[256];
        if (fgets(comm, sizeof(comm), f) != NULL) {
            size_t len = strlen(comm);
            if (len > 0 && comm[len - 1] == '\n') comm[len - 1] = '\0';

            if (strcmp(comm, argv[1]) == 0) {
                if (found) printf(" ");
                printf("%s", ent->d_name);
                found = 1;
            }
        }
        fclose(f);
    }
    closedir(d);

    if (found) printf("\n");
    return found ? 0 : 1;
}
