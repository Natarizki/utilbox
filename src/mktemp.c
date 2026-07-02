/* mktemp.c — create a unique temporary file.
 * Usage: mktemp [TEMPLATE]   (TEMPLATE must end with XXXXXX, default
 * "/tmp/tmp.XXXXXX") */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utilbox.h"

int mktemp_main(int argc, char **argv)
{
    char templ[4096];

    if (argc >= 2) {
        strncpy(templ, argv[1], sizeof(templ) - 1);
        templ[sizeof(templ) - 1] = '\0';
    } else {
        strcpy(templ, "/tmp/tmp.XXXXXX");
    }

    int fd = mkstemp(templ);
    if (fd < 0) {
        return die("mktemp", "cannot create temp file", templ);
    }
    close(fd);

    puts(templ);
    return 0;
}
