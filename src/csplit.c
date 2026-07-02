/* csplit.c — split a file into two parts at a given line number.
 * Usage: csplit FILE LINENUM  (creates xx00 and xx01) */
#include <stdio.h>
#include <stdlib.h>
#include "utilbox.h"

int csplit_main(int argc, char **argv)
{
    if (argc != 3) {
        return fail("csplit", "usage: csplit FILE LINENUM");
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) return die("csplit", "cannot open", argv[1]);

    long splitline = atol(argv[2]);

    FILE *out1 = fopen("xx00", "w");
    FILE *out2 = fopen("xx01", "w");
    if (!out1 || !out2) {
        fclose(in);
        return die("csplit", "cannot create output files", NULL);
    }

    char line[65536];
    long n = 0;
    while (fgets(line, sizeof(line), in) != NULL) {
        n++;
        if (n < splitline) fputs(line, out1);
        else fputs(line, out2);
    }

    fclose(in);
    fclose(out1);
    fclose(out2);

    printf("xx00\nxx01\n");
    return 0;
}
