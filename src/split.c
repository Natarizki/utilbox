/* split.c — split a file into pieces by line count.
 * Usage: split [-l LINES] FILE [PREFIX]  (default LINES=1000, PREFIX="x") */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilbox.h"

int split_main(int argc, char **argv)
{
    long lines_per_file = 1000;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
        lines_per_file = strtol(argv[i + 1], NULL, 10);
        i += 2;
    }

    if (i >= argc) {
        return fail("split", "usage: split [-l LINES] FILE [PREFIX]");
    }

    const char *infile = argv[i++];
    const char *prefix = (i < argc) ? argv[i] : "x";

    FILE *in = fopen(infile, "r");
    if (!in) return die("split", "cannot open", infile);

    char line[65536];
    int part = 0;
    long count = 0;
    FILE *out = NULL;

    while (fgets(line, sizeof(line), in) != NULL) {
        if (out == NULL || count >= lines_per_file) {
            if (out) fclose(out);
            char outname[512];
            snprintf(outname, sizeof(outname), "%s%02d", prefix, part++);
            out = fopen(outname, "w");
            if (!out) {
                fclose(in);
                return die("split", "cannot create", outname);
            }
            count = 0;
        }
        fputs(line, out);
        count++;
    }

    if (out) fclose(out);
    fclose(in);
    return 0;
}
