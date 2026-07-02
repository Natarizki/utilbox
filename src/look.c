/* look.c — search a (sorted) file for lines beginning with a given prefix */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int look_main(int argc, char **argv)
{
    if (argc != 3) {
        return fail("look", "usage: look PREFIX FILE");
    }

    FILE *f = fopen(argv[2], "r");
    if (!f) return die("look", "cannot open", argv[2]);

    size_t plen = strlen(argv[1]);
    char line[65536];
    int found = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        if (strncmp(line, argv[1], plen) == 0) {
            fputs(line, stdout);
            found = 1;
        }
    }

    fclose(f);
    return found ? 0 : 1;
}
