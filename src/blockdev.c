/* blockdev.c — query the size of a block device.
 * Usage: blockdev --getsize64 DEVICE */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include "utilbox.h"

int blockdev_main(int argc, char **argv)
{
    if (argc != 3 || strcmp(argv[1], "--getsize64") != 0) {
        return fail("blockdev", "usage: blockdev --getsize64 DEVICE");
    }

    int fd = open(argv[2], O_RDONLY);
    if (fd < 0) return die("blockdev", "cannot open", argv[2]);

    unsigned long long size = 0;
    if (ioctl(fd, BLKGETSIZE64, &size) != 0) {
        close(fd);
        return die("blockdev", "cannot query size (need privilege)", argv[2]);
    }

    close(fd);
    printf("%llu\n", size);
    return 0;
}
