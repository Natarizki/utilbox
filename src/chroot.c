/* chroot.c — change root directory, then exec a command (default: sh) */
#include <stdio.h>
#include <unistd.h>
#include "utilbox.h"

int chroot_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("chroot", "usage: chroot NEWROOT [COMMAND [ARGS...]]");
    }

    if (chroot(argv[1]) != 0) {
        return die("chroot", "cannot chroot to", argv[1]);
    }

    if (chdir("/") != 0) {
        return die("chroot", "cannot chdir to /", NULL);
    }

    if (argc >= 3) {
        execvp(argv[2], argv + 2);
        return die("chroot", "cannot execute", argv[2]);
    }

    char *sh_argv[] = { "/bin/sh", NULL };
    execvp(sh_argv[0], sh_argv);
    return die("chroot", "cannot execute", "/bin/sh");
}
