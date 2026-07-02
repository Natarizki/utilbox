/* basename.c — strip directory (and optional suffix) from a pathname. */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int basename_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("basename", "usage: basename PATH [SUFFIX]");
    }

    char buf[4096];
    strncpy(buf, argv[1], sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    size_t len = strlen(buf);
    while (len > 1 && buf[len - 1] == '/') {
        buf[--len] = '\0';
    }

    char *base = strrchr(buf, '/');
    base = base ? base + 1 : buf;

    if (argc >= 3) {
        size_t blen = strlen(base);
        size_t slen = strlen(argv[2]);
        if (blen > slen && strcmp(base + blen - slen, argv[2]) == 0) {
            base[blen - slen] = '\0';
        }
    }

    puts(base);
    return 0;
}
