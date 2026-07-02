/* uptime.c — display how long the system has been running, from /proc/uptime */
#include <stdio.h>
#include "utilbox.h"

int uptime_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    FILE *f = fopen("/proc/uptime", "r");
    if (!f) {
        return die("uptime", "cannot open /proc/uptime", NULL);
    }

    double secs;
    if (fscanf(f, "%lf", &secs) != 1) {
        fclose(f);
        return fail("uptime", "cannot parse /proc/uptime");
    }
    fclose(f);

    long total = (long)secs;
    long days = total / 86400;
    long hours = (total % 86400) / 3600;
    long mins = (total % 3600) / 60;

    printf("up ");
    if (days > 0) printf("%ld days, ", days);
    printf("%ld:%02ld\n", hours, mins);

    return 0;
}
