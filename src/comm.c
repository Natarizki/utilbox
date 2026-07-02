/* comm.c — compare two sorted files line by line.
 * Prints 3 columns: unique to FILE1, unique to FILE2, common to both.
 * Simplified: no -1/-2/-3 suppression flags in v1. */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int comm_main(int argc, char **argv)
{
    if (argc != 3) {
        return fail("comm", "usage: comm FILE1 FILE2");
    }

    FILE *f1 = fopen(argv[1], "r");
    if (!f1) return die("comm", "cannot open", argv[1]);
    FILE *f2 = fopen(argv[2], "r");
    if (!f2) { fclose(f1); return die("comm", "cannot open", argv[2]); }

    char l1[4096], l2[4096];
    int have1 = (fgets(l1, sizeof(l1), f1) != NULL);
    int have2 = (fgets(l2, sizeof(l2), f2) != NULL);

    while (have1 || have2) {
        int cmp;
        if (!have1) cmp = 1;
        else if (!have2) cmp = -1;
        else cmp = strcmp(l1, l2);

        if (cmp < 0) {
            fputs(l1, stdout);
            have1 = (fgets(l1, sizeof(l1), f1) != NULL);
        }
        else if (cmp > 0) {
            printf("\t%s", l2);
            have2 = (fgets(l2, sizeof(l2), f2) != NULL);
        }
        else {
            printf("\t\t%s", l1);
            have1 = (fgets(l1, sizeof(l1), f1) != NULL);
            have2 = (fgets(l2, sizeof(l2), f2) != NULL);
        }
    }

    fclose(f1);
    fclose(f2);
    return 0;
}
