/* dirname.c — strip the last component from a pathname. */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int dirname_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("dirname", "usage: dirname PATH");
    }

    char buf[4096];
    strncpy(buf, argv[1], sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    size_t len = strlen(buf);
    while (len > 1 && buf[len - 1] == '/') {
        buf[--len] = '\0';
    }

    char *slash = strrchr(buf, '/');
    if (slash == NULL) {
        puts(".");
    }
    else if (slash == buf) {
        puts("/");
    }
    else {
        *slash = '\0';
        puts(buf);
    }

    return 0;
}
