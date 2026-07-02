/* tail.c — print the last N lines of a file (default 10). Supports -n NUM.
 * Simple implementation: reads all lines into memory then prints the tail. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilbox.h"

static int tail_stream(FILE *in, long n)
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

    size_t start = (count > (size_t)n) ? count - (size_t)n : 0;
    for (size_t i = start; i < count; i++) {
        fputs(lines[i], stdout);
        free(lines[i]);
    }
    for (size_t i = 0; i < start; i++) free(lines[i]);
    free(lines);

    return 0;
}

int tail_main(int argc, char **argv)
{
    long n = 10;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
        n = strtol(argv[i + 1], NULL, 10);
        i += 2;
    }

    if (i >= argc) {
        return tail_stream(stdin, n);
    }

    int rc = 0;
    for (; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("tail", "cannot open", argv[i]);
            continue;
        }
        if (argc - i > 1) printf("==> %s <==\n", argv[i]);
        rc |= tail_stream(f, n);
        fclose(f);
    }

    return rc;
}
