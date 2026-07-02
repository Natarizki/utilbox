/* tee.c — read stdin, write to stdout and to the named file(s).
 * Supports -a (append instead of truncate). */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

#define MAX_FILES 16

int tee_main(int argc, char **argv)
{
    int append = 0;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-a") == 0) {
        append = 1;
        i++;
    }

    FILE *outs[MAX_FILES];
    int nouts = 0;

    for (; i < argc && nouts < MAX_FILES; i++) {
        FILE *f = fopen(argv[i], append ? "a" : "w");
        if (!f) {
            die("tee", "cannot open", argv[i]);
            continue;
        }
        outs[nouts++] = f;
    }

    char buf[8192];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), stdin)) > 0) {
        fwrite(buf, 1, n, stdout);
        for (int j = 0; j < nouts; j++) {
            fwrite(buf, 1, n, outs[j]);
        }
    }

    for (int j = 0; j < nouts; j++) fclose(outs[j]);

    return 0;
}
