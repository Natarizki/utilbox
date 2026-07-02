/* column.c — arrange input words into aligned columns */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilbox.h"

#define MAX_WORDS 8192
#define COL_WIDTH 3

int column_main(int argc, char **argv)
{
    FILE *in = stdin;
    if (argc >= 2) {
        in = fopen(argv[1], "r");
        if (!in) return die("column", "cannot open", argv[1]);
    }

    char *words[MAX_WORDS];
    int nwords = 0;
    char buf[4096];

    while (nwords < MAX_WORDS && fscanf(in, "%4095s", buf) == 1) {
        words[nwords++] = strdup(buf);
    }
    if (in != stdin) fclose(in);

    int maxlen = 0;
    for (int i = 0; i < nwords; i++) {
        int len = (int)strlen(words[i]);
        if (len > maxlen) maxlen = len;
    }

    int termwidth = 80;
    int colwidth = maxlen + COL_WIDTH;
    int ncols = termwidth / colwidth;
    if (ncols < 1) ncols = 1;

    for (int i = 0; i < nwords; i++) {
        printf("%-*s", colwidth, words[i]);
        free(words[i]);
        if ((i + 1) % ncols == 0) printf("\n");
    }
    if (nwords % ncols != 0) printf("\n");

    return 0;
}
