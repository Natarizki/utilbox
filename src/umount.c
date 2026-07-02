/* umount.c — unmount a filesystem */
#include <sys/mount.h>
#include "utilbox.h"

int umount_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("umount", "usage: umount TARGET");
    }

    if (umount(argv[1]) != 0) {
        return die("umount", "cannot unmount", argv[1]);
    }

    return 0;
}
