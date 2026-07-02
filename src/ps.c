/* ps.c — list running processes by reading /proc/<pid>/stat.
 * Simplified: shows PID, state, and command name only (no full column set). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include "utilbox.h"

int ps_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    DIR *d = opendir("/proc");
    if (!d) {
        return die("ps", "cannot open /proc", NULL);
    }

    printf("%6s %-2s %s\n", "PID", "ST", "CMD");

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (!isdigit((unsigned char)ent->d_name[0])) continue;

        char path[64];
        snprintf(path, sizeof(path), "/proc/%s/stat", ent->d_name);
        FILE *f = fopen(path, "r");
        if (!f) continue;

        int pid;
        char comm[256];
        char state;
        if (fscanf(f, "%d (%255[^)]) %c", &pid, comm, &state) == 3) {
            printf("%6d %-2c %s\n", pid, state, comm);
        }
        fclose(f);
    }

    closedir(d);
    return 0;
}
