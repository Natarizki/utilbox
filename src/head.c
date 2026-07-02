/* head.c — print the first N lines of a file (default 10). Supports -n NUM. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilbox.h"

static int head_stream(FILE *in, long n)
{
    char line[65536];
    long count = 0;
    while (count < n && fgets(line, sizeof(line), in) != NULL) {
        fputs(line, stdout);
        count++;
    }
    return 0;
}

int head_main(int argc, char **argv)
{
    long n = 10;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
        n = strtol(argv[i + 1], NULL, 10);
        i += 2;
    }

    if (i >= argc) {
        return head_stream(stdin, n);
    }

    int rc = 0;
    for (; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("head", "cannot open", argv[i]);
            continue;
        }
        if (argc - i > 1) printf("==> %s <==\n", argv[i]);
        rc |= head_stream(f, n);
        fclose(f);
    }

    return rc;
}
