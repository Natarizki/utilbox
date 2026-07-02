/* cat.c — concatenate files and print to stdout.
 * With no arguments, reads from stdin. */
#include <stdio.h>
#include <unistd.h>
#include "utilbox.h"

static int cat_stream(FILE *in, const char *name)
{
    char buf[8192];
    size_t n;

    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (fwrite(buf, 1, n, stdout) != n) {
            return die("cat", "write error", name);
        }
    }

    if (ferror(in)) {
        return die("cat", "read error", name);
    }

    return 0;
}

int cat_main(int argc, char **argv)
{
    if (argc < 2) {
        return cat_stream(stdin, "-");
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-") == 0) {
            rc |= cat_stream(stdin, "-");
            continue;
        }

        FILE *f = fopen(argv[i], "rb");
        if (!f) {
            rc |= die("cat", "cannot open", argv[i]);
            continue;
        }

        rc |= cat_stream(f, argv[i]);
        fclose(f);
    }

    return rc;
}
