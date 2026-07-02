/* nohup.c — run a command immune to SIGHUP, redirecting output to
 * nohup.out if stdout is a terminal. */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include "utilbox.h"

int nohup_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("nohup", "usage: nohup COMMAND [ARGS...]");
    }

    signal(SIGHUP, SIG_IGN);

    if (isatty(1)) {
        int fd = open("nohup.out", O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd >= 0) {
            dup2(fd, 1);
            close(fd);
        }
    }

    execvp(argv[1], argv + 1);
    return die("nohup", "cannot execute", argv[1]);
}
