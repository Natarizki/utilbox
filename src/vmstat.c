/* vmstat.c — simplified virtual memory statistics, from /proc/meminfo.
 * Not the full column set of real vmstat. */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int vmstat_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    long mem_total = 0, mem_free = 0, mem_buffers = 0, mem_cached = 0, swap_total = 0, swap_free = 0;

    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) return die("vmstat", "cannot open /proc/meminfo", NULL);

    char line[256], key[64];
    long val;
    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "%63[^:]: %ld", key, &val) == 2) {
            if (strcmp(key, "MemTotal") == 0) mem_total = val;
            else if (strcmp(key, "MemFree") == 0) mem_free = val;
            else if (strcmp(key, "Buffers") == 0) mem_buffers = val;
            else if (strcmp(key, "Cached") == 0) mem_cached = val;
            else if (strcmp(key, "SwapTotal") == 0) swap_total = val;
            else if (strcmp(key, "SwapFree") == 0) swap_free = val;
        }
    }
    fclose(f);

    printf("--memory (KB)--\n");
    printf("total    free     buff     cache    swpd\n");
    printf("%-8ld %-8ld %-8ld %-8ld %-8ld\n",
            mem_total, mem_free, mem_buffers, mem_cached, swap_total - swap_free);

    return 0;
}
