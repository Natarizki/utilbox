/* readlink.c — print the target of a symbolic link */
#include <stdio.h>
#include <unistd.h>
#include "utilbox.h"

int readlink_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("readlink", "usage: readlink LINK");
    }

    char buf[4096];
    ssize_t n = readlink(argv[1], buf, sizeof(buf) - 1);
    if (n < 0) {
        return die("readlink", "cannot read link", argv[1]);
    }

    buf[n] = '\0';
    puts(buf);
    return 0;
}
