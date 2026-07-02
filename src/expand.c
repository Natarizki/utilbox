/* expand.c — convert tabs to spaces (default tab stop: 8) */
#include <stdio.h>
#include "utilbox.h"

static void expand_stream(FILE *in, int tabstop)
{
    int c;
    int col = 0;
    while ((c = fgetc(in)) != EOF) {
        if (c == '\t') {
            int spaces = tabstop - (col % tabstop);
            for (int i = 0; i < spaces; i++) { putchar(' '); col++; }
        } else {
            putchar(c);
            col = (c == '\n') ? 0 : col + 1;
        }
    }
}

int expand_main(int argc, char **argv)
{
    int tabstop = 8;

    if (argc < 2) {
        expand_stream(stdin, tabstop);
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("expand", "cannot open", argv[i]);
            continue;
        }
        expand_stream(f, tabstop);
        fclose(f);
    }

    return rc;
}
