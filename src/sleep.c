/* sleep.c — pause for a specified number of seconds (fractional allowed). */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utilbox.h"

int sleep_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("sleep", "usage: sleep SECONDS");
    }

    double secs = atof(argv[1]);
    struct timespec ts;
    ts.tv_sec = (time_t)secs;
    ts.tv_nsec = (long)((secs - ts.tv_sec) * 1e9);

    if (nanosleep(&ts, NULL) != 0) {
        return die("sleep", "interrupted", NULL);
    }

    return 0;
}
