/* install.c — copy a file to a destination, setting its mode.
 * Usage: install [-m MODE] SOURCE DEST */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "utilbox.h"

int install_main(int argc, char **argv)
{
    mode_t mode = 0755;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
        mode = (mode_t)strtol(argv[i + 1], NULL, 8);
        i += 2;
    }

    if (i + 2 > argc) {
        return fail("install", "usage: install [-m MODE] SOURCE DEST");
    }

    const char *src = argv[i];
    const char *dst = argv[i + 1];

    int in = open(src, O_RDONLY);
    if (in < 0) return die("install", "cannot open", src);

    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (out < 0) {
        close(in);
        return die("install", "cannot create", dst);
    }

    char buf[8192];
    ssize_t n;
    while ((n = read(in, buf, sizeof(buf))) > 0) {
        write(out, buf, n);
    }

    close(in);
    close(out);
    chmod(dst, mode);

    return 0;
}
