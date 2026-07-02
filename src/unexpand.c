/* unexpand.c — convert leading spaces back to tabs (tab stop: 8) */
#include <stdio.h>
#include "utilbox.h"

static void unexpand_stream(FILE *in, int tabstop)
{
    char line[65536];
    while (fgets(line, sizeof(line), in) != NULL) {
        int i = 0, spaces = 0;
        while (line[i] == ' ') { spaces++; i++; }

        int tabs = spaces / tabstop;
        int rem = spaces % tabstop;

        for (int t = 0; t < tabs; t++) putchar('\t');
        for (int s = 0; s < rem; s++) putchar(' ');

        fputs(line + i, stdout);
    }
}

int unexpand_main(int argc, char **argv)
{
    int tabstop = 8;

    if (argc < 2) {
        unexpand_stream(stdin, tabstop);
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("unexpand", "cannot open", argv[i]);
            continue;
        }
        unexpand_stream(f, tabstop);
        fclose(f);
    }

    return rc;
}
