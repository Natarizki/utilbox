/* mkdir.c — create directories.
 * Supports -p (create parent directories as needed) and -m MODE. */
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "utilbox.h"

static int mkdir_p(const char *path, mode_t mode)
{
    char tmp[4096];
    strncpy(tmp, path, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    size_t len = strlen(tmp);
    if (len > 0 && tmp[len - 1] == '/') tmp[len - 1] = '\0';

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, mode) != 0 && errno != EEXIST) {
                return die("mkdir", "cannot create directory", tmp);
            }
            *p = '/';
        }
    }

    if (mkdir(tmp, mode) != 0 && errno != EEXIST) {
        return die("mkdir", "cannot create directory", tmp);
    }

    return 0;
}

int mkdir_main(int argc, char **argv)
{
    int parents = 0;
    mode_t mode = 0777;
    int i = 1;

    for (; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            parents = 1;
        }
        else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            mode = (mode_t)strtol(argv[++i], NULL, 8);
        }
        else {
            break;
        }
    }

    if (i >= argc) {
        return fail("mkdir", "missing operand");
    }

    int rc = 0;
    for (; i < argc; i++) {
        if (parents) {
            rc |= mkdir_p(argv[i], mode);
        }
        else if (mkdir(argv[i], mode) != 0) {
            rc |= die("mkdir", "cannot create directory", argv[i]);
        }
    }

    return rc;
}
