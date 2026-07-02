/* uniq.c — remove consecutive duplicate lines (like the input should be
 * sorted first, same as standard uniq behavior). */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

static int uniq_stream(FILE *in)
{
    char prev[65536];
    char line[65536];
    int have_prev = 0;

    while (fgets(line, sizeof(line), in) != NULL) {
        if (!have_prev || strcmp(prev, line) != 0) {
            fputs(line, stdout);
            strcpy(prev, line);
            have_prev = 1;
        }
    }

    return 0;
}

int uniq_main(int argc, char **argv)
{
    if (argc < 2) {
        return uniq_stream(stdin);
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("uniq", "cannot open", argv[i]);
            continue;
        }
        rc |= uniq_stream(f);
        fclose(f);
    }

    return rc;
}
