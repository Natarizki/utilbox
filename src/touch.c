/* touch.c — create empty files, or update timestamp of existing ones */
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>
#include "utilbox.h"

int touch_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("touch", "missing operand");
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_WRONLY | O_CREAT, 0666);
        if (fd < 0) {
            rc |= die("touch", "cannot touch", argv[i]);
            continue;
        }
        close(fd);

        if (utime(argv[i], NULL) != 0) {
            rc |= die("touch", "cannot update timestamp", argv[i]);
        }
    }

    return rc;
}
