/* tac.c — concatenate and print files in reverse line order */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilbox.h"

static int tac_stream(FILE *in)
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

    for (size_t i = count; i > 0; i--) {
        fputs(lines[i - 1], stdout);
        free(lines[i - 1]);
    }
    free(lines);

    return 0;
}

int tac_main(int argc, char **argv)
{
    if (argc < 2) {
        return tac_stream(stdin);
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("tac", "cannot open", argv[i]);
            continue;
        }
        rc |= tac_stream(f);
        fclose(f);
    }

    return rc;
}
