/* nl.c — number lines of a file */
#include <stdio.h>
#include "utilbox.h"

static void nl_stream(FILE *in)
{
    char line[65536];
    long n = 1;
    while (fgets(line, sizeof(line), in) != NULL) {
        printf("%6ld\t%s", n++, line);
    }
}

int nl_main(int argc, char **argv)
{
    if (argc < 2) {
        nl_stream(stdin);
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("nl", "cannot open", argv[i]);
            continue;
        }
        nl_stream(f);
        fclose(f);
    }

    return rc;
}
