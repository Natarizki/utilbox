/* wc.c — count lines, words, and bytes. Supports -l, -w, -c (default: all three). */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

static void count_stream(FILE *in, long *lines, long *words, long *bytes)
{
    int c;
    int in_word = 0;
    *lines = *words = *bytes = 0;

    while ((c = fgetc(in)) != EOF) {
        (*bytes)++;
        if (c == '\n') (*lines)++;
        if (c == ' ' || c == '\t' || c == '\n') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            (*words)++;
        }
    }
}

int wc_main(int argc, char **argv)
{
    int show_l = 0, show_w = 0, show_c = 0;
    int i = 1;

    for (; i < argc && argv[i][0] == '-'; i++) {
        if (strchr(argv[i], 'l')) show_l = 1;
        if (strchr(argv[i], 'w')) show_w = 1;
        if (strchr(argv[i], 'c')) show_c = 1;
    }

    if (!show_l && !show_w && !show_c) {
        show_l = show_w = show_c = 1;
    }

    long total_l = 0, total_w = 0, total_c = 0;
    int rc = 0;

    if (i >= argc) {
        long l, w, c;
        count_stream(stdin, &l, &w, &c);
        if (show_l) printf("%7ld ", l);
        if (show_w) printf("%7ld ", w);
        if (show_c) printf("%7ld ", c);
        printf("\n");
        return 0;
    }

    for (; i < argc; i++) {
        FILE *f = fopen(argv[i], "rb");
        if (!f) {
            rc |= die("wc", "cannot open", argv[i]);
            continue;
        }
        long l, w, c;
        count_stream(f, &l, &w, &c);
        fclose(f);

        if (show_l) printf("%7ld ", l);
        if (show_w) printf("%7ld ", w);
        if (show_c) printf("%7ld ", c);
        printf("%s\n", argv[i]);

        total_l += l; total_w += w; total_c += c;
    }

    if (argc - 1 - (i - argc) > 1) {
        /* multiple files were processed, print totals - simplified check: */
    }

    return rc;
}
