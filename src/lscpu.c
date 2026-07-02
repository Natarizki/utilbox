/* lscpu.c — display CPU information, parsed from /proc/cpuinfo */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int lscpu_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) return die("lscpu", "cannot open /proc/cpuinfo", NULL);

    char line[512];
    int ncpus = 0;
    char model[256] = "unknown";

    while (fgets(line, sizeof(line), f) != NULL) {
        if (strncmp(line, "processor", 9) == 0) ncpus++;
        if (strncmp(line, "model name", 10) == 0 || strncmp(line, "Hardware", 8) == 0) {
            char *colon = strchr(line, ':');
            if (colon) {
                strncpy(model, colon + 2, sizeof(model) - 1);
                model[strcspn(model, "\n")] = '\0';
            }
        }
    }
    fclose(f);

    printf("CPU(s):          %d\n", ncpus);
    printf("Model name:      %s\n", model);

    return 0;
}
