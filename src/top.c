/* top.c — one-shot snapshot of processes, sorted by memory usage (RSS) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "utilbox.h"

typedef struct { int pid; long rss; char comm[256]; } proc_info;

static int cmp_rss(const void *a, const void *b)
{
    const proc_info *pa = a, *pb = b;
    return (pb->rss > pa->rss) - (pb->rss < pa->rss);
}

int top_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    DIR *d = opendir("/proc");
    if (!d) return die("top", "cannot open /proc", NULL);

    proc_info procs[512];
    int n = 0;

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL && n < 512) {
        if (!isdigit((unsigned char)ent->d_name[0])) continue;

        char path[64];
        snprintf(path, sizeof(path), "/proc/%s/status", ent->d_name);
        FILE *f = fopen(path, "r");
        if (!f) continue;

        char line[256];
        long rss = 0;
        char name[256] = "?";
        while (fgets(line, sizeof(line), f) != NULL) {
            if (strncmp(line, "VmRSS:", 6) == 0) sscanf(line, "VmRSS: %ld", &rss);
            if (strncmp(line, "Name:", 5) == 0) sscanf(line, "Name: %255s", name);
        }
        fclose(f);

        procs[n].pid = atoi(ent->d_name);
        procs[n].rss = rss;
        strcpy(procs[n].comm, name);
        n++;
    }
    closedir(d);

    qsort(procs, n, sizeof(proc_info), cmp_rss);

    printf("%8s %10s  %s\n", "PID", "RSS(KB)", "COMMAND");
    int limit = (n < 15) ? n : 15;
    for (int i = 0; i < limit; i++) {
        printf("%8d %10ld  %s\n", procs[i].pid, procs[i].rss, procs[i].comm);
    }

    return 0;
}
