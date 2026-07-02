/* fold.c — wrap each line to a specified width (default 80 chars). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilbox.h"

static void fold_stream(FILE *in, int width)
{
    char line[65536];
    while (fgets(line, sizeof(line), in) != NULL) {
        size_t len = strlen(line);
        int has_nl = (len > 0 && line[len - 1] == '\n');
        if (has_nl) len--;

        size_t pos = 0;
        while (pos < len) {
            size_t chunk = (len - pos > (size_t)width) ? (size_t)width : (len - pos);
            fwrite(line + pos, 1, chunk, stdout);
            putchar('\n');
            pos += chunk;
        }
        if (len == 0 && has_nl) putchar('\n');
    }
}

int fold_main(int argc, char **argv)
{
    int width = 80;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
        width = atoi(argv[i + 1]);
        i += 2;
    }

    if (i >= argc) {
        fold_stream(stdin, width);
        return 0;
    }

    int rc = 0;
    for (; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("fold", "cannot open", argv[i]);
            continue;
        }
        fold_stream(f, width);
        fclose(f);
    }

    return rc;
}
