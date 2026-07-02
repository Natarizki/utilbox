/* mount.c — mount a filesystem. Usage: mount [-t TYPE] SOURCE TARGET */
#include <stdio.h>
#include <string.h>
#include <sys/mount.h>
#include "utilbox.h"

int mount_main(int argc, char **argv)
{
    const char *type = NULL;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
        type = argv[i + 1];
        i += 2;
    }

    if (i + 2 > argc) {
        return fail("mount", "usage: mount [-t TYPE] SOURCE TARGET");
    }

    const char *source = argv[i];
    const char *target = argv[i + 1];

    if (mount(source, target, type, 0, NULL) != 0) {
        return die("mount", "cannot mount", source);
    }

    return 0;
}
