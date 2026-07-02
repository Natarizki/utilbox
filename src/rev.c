/* rev.c — reverse the characters of each line */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

static void rev_stream(FILE *in)
{
    char line[65536];
    while (fgets(line, sizeof(line), in) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') len--;
        for (size_t i = len; i > 0; i--) {
            putchar(line[i - 1]);
        }
        putchar('\n');
    }
}

int rev_main(int argc, char **argv)
{
    if (argc < 2) {
        rev_stream(stdin);
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("rev", "cannot open", argv[i]);
            continue;
        }
        rev_stream(f);
        fclose(f);
    }

    return rc;
}
