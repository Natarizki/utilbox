/* which.c — locate an executable in $PATH */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utilbox.h"

int which_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("which", "usage: which COMMAND");
    }

    const char *path_env = getenv("PATH");
    if (!path_env) path_env = "/usr/bin:/bin";

    char pathbuf[4096];
    strncpy(pathbuf, path_env, sizeof(pathbuf) - 1);
    pathbuf[sizeof(pathbuf) - 1] = '\0';

    int found = 0;
    char *dir = strtok(pathbuf, ":");
    while (dir != NULL) {
        char full[4096];
        snprintf(full, sizeof(full), "%s/%s", dir, argv[1]);
        if (access(full, X_OK) == 0) {
            puts(full);
            found = 1;
            break;
        }
        dir = strtok(NULL, ":");
    }

    return found ? 0 : 1;
}
