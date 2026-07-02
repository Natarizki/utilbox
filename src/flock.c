/* flock.c — acquire an exclusive lock on a file while running a command.
 * Usage: flock FILE COMMAND [ARGS...] */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include "utilbox.h"

int flock_main(int argc, char **argv)
{
    if (argc < 3) {
        return fail("flock", "usage: flock FILE COMMAND [ARGS...]");
    }

    int fd = open(argv[1], O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        return die("flock", "cannot open", argv[1]);
    }

    if (flock(fd, LOCK_EX) != 0) {
        close(fd);
        return die("flock", "cannot acquire lock", argv[1]);
    }

    execvp(argv[2], argv + 2);
    return die("flock", "cannot execute", argv[2]);
}
