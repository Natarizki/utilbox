/* sort.c — sort lines of text. Supports -r (reverse). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilbox.h"

static int cmp_asc(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

static int cmp_desc(const void *a, const void *b)
{
    return strcmp(*(const char **)b, *(const char **)a);
}

static int sort_stream(FILE *in, int reverse)
{
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

    qsort(lines, count, sizeof(char *), reverse ? cmp_desc : cmp_asc);

    for (size_t i = 0; i < count; i++) {
        fputs(lines[i], stdout);
        free(lines[i]);
    }
    free(lines);

    return 0;
}

int sort_main(int argc, char **argv)
{
    int reverse = 0;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-r") == 0) {
        reverse = 1;
        i++;
    }

    if (i >= argc) {
        return sort_stream(stdin, reverse);
    }

    int rc = 0;
    for (; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("sort", "cannot open", argv[i]);
            continue;
        }
        rc |= sort_stream(f, reverse);
        fclose(f);
    }

    return rc;
}
