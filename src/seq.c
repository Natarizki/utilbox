/* seq.c — print a sequence of numbers.
 * Usage: seq LAST            (1..LAST)
 *        seq FIRST LAST      (FIRST..LAST)
 *        seq FIRST STEP LAST (FIRST, FIRST+STEP, ... up to LAST) */
#include <stdio.h>
#include <stdlib.h>
#include "utilbox.h"

int seq_main(int argc, char **argv)
{
    long first = 1, step = 1, last;

    if (argc == 2) {
        last = atol(argv[1]);
    }
    else if (argc == 3) {
        first = atol(argv[1]);
        last = atol(argv[2]);
    }
    else if (argc == 4) {
        first = atol(argv[1]);
        step = atol(argv[2]);
        last = atol(argv[3]);
    }
    else {
        return fail("seq", "usage: seq [FIRST [STEP]] LAST");
    }

    if (step == 0) {
        return fail("seq", "STEP cannot be zero");
    }

    if (step > 0) {
        for (long i = first; i <= last; i += step) {
            printf("%ld\n", i);
        }
    } else {
        for (long i = first; i >= last; i += step) {
            printf("%ld\n", i);
        }
    }

    return 0;
}
