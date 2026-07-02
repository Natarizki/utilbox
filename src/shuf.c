/* shuf.c — randomly shuffle lines of input (Fisher-Yates) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utilbox.h"

int shuf_main(int argc, char **argv)
{
    FILE *in = stdin;
    if (argc >= 2) {
        in = fopen(argv[1], "r");
        if (!in) return die("shuf", "cannot open", argv[1]);
    }

    char **lines = NULL;
    size_t count = 0, cap = 0;
    char buf[65536];

    while (fgets(buf, sizeof(buf), in) != NULL) {
        if (count == cap) {
            cap = cap ? cap * 2 : 64;
            lines = realloc(lines, cap * sizeof(char *));
        }
        lines[count++] = strdup(buf);
    }
    if (in != stdin) fclose(in);

    srand((unsigned)(time(NULL) ^ getpid()));

    for (size_t i = count; i > 1; i--) {
        size_t j = rand() % i;
        char *tmp = lines[i - 1];
        lines[i - 1] = lines[j];
        lines[j] = tmp;
    }

    for (size_t i = 0; i < count; i++) {
        fputs(lines[i], stdout);
        free(lines[i]);
    }
    free(lines);

    return 0;
}
