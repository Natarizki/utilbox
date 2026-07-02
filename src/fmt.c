/* fmt.c — reformat text paragraphs to a given width (default 75), simple
 * greedy word-wrap. Blank lines separate paragraphs. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilbox.h"

static void fmt_stream(FILE *in, int width)
{
    char word[4096];
    int col = 0;
    int c;
    size_t wlen = 0;

    while ((c = fgetc(in)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (wlen > 0) {
                word[wlen] = '\0';
                if (col > 0 && col + 1 + (int)wlen > width) {
                    putchar('\n');
                    col = 0;
                }
                if (col > 0) {
                    putchar(' ');
                    col++;
                }
                fputs(word, stdout);
                col += wlen;
                wlen = 0;
            }
            if (c == '\n') {
                int next = fgetc(in);
                if (next == '\n') {
                    if (col > 0) putchar('\n');
                    putchar('\n');
                    col = 0;
                } else if (next != EOF) {
                    ungetc(next, in);
                }
            }
        }
        else if (wlen < sizeof(word) - 1) {
            word[wlen++] = (char)c;
        }
    }

    if (wlen > 0) {
        word[wlen] = '\0';
        if (col > 0 && col + 1 + (int)wlen > width) putchar('\n');
        else if (col > 0) putchar(' ');
        fputs(word, stdout);
        col += wlen;
    }
    if (col > 0) putchar('\n');
}

int fmt_main(int argc, char **argv)
{
    int width = 75;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
        width = atoi(argv[i + 1]);
        i += 2;
    }

    if (i >= argc) {
        fmt_stream(stdin, width);
        return 0;
    }

    int rc = 0;
    for (; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("fmt", "cannot open", argv[i]);
            continue;
        }
        fmt_stream(f, width);
        fclose(f);
    }

    return rc;
}
