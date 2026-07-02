/* free.c — display memory usage, parsed from /proc/meminfo */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int free_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) {
        return die("free", "cannot open /proc/meminfo", NULL);
    }

    long total = 0, free_kb = 0, available = 0, buffers = 0, cached = 0;
    char line[256], key[64];
    long val;

    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "%63[^:]: %ld", key, &val) == 2) {
            if (strcmp(key, "MemTotal") == 0) total = val;
            else if (strcmp(key, "MemFree") == 0) free_kb = val;
            else if (strcmp(key, "MemAvailable") == 0) available = val;
            else if (strcmp(key, "Buffers") == 0) buffers = val;
            else if (strcmp(key, "Cached") == 0) cached = val;
        }
    }
    fclose(f);

    long used = total - free_kb - buffers - cached;

    printf("%-10s %10s %10s %10s %10s\n", "", "total", "used", "free", "available");
    printf("%-10s %10ld %10ld %10ld %10ld\n", "Mem:", total, used, free_kb, available);

    return 0;
}
