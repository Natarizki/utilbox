/* losetup.c — associate a file with a loop device.
 * Usage: losetup LOOPDEV FILE */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/loop.h>
#include "utilbox.h"

int losetup_main(int argc, char **argv)
{
    if (argc != 3) {
        return fail("losetup", "usage: losetup LOOPDEV FILE");
    }

    int loopfd = open(argv[1], O_RDWR);
    if (loopfd < 0) return die("losetup", "cannot open loop device", argv[1]);

    int filefd = open(argv[2], O_RDWR);
    if (filefd < 0) {
        close(loopfd);
        return die("losetup", "cannot open backing file", argv[2]);
    }

    if (ioctl(loopfd, LOOP_SET_FD, filefd) != 0) {
        close(loopfd);
        close(filefd);
        return die("losetup", "cannot associate loop device (need privilege)", NULL);
    }

    close(filefd);
    close(loopfd);
    return 0;
}
