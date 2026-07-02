/* type.c — indicate how a command name would be interpreted:
 * a built-in utilbox applet, or an external file in $PATH. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utilbox.h"
#include "applets.h"

int type_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("type", "usage: type NAME");
    }

    for (int i = 0; applets[i].name != NULL; i++) {
        if (strcmp(applets[i].name, argv[1]) == 0) {
            printf("%s is a utilbox built-in applet\n", argv[1]);
            return 0;
        }
    }

    const char *path_env = getenv("PATH");
    if (!path_env) path_env = "/usr/bin:/bin";

    char pathbuf[4096];
    strncpy(pathbuf, path_env, sizeof(pathbuf) - 1);
    pathbuf[sizeof(pathbuf) - 1] = '\0';

    char *dir = strtok(pathbuf, ":");
    while (dir != NULL) {
        char full[4096];
        snprintf(full, sizeof(full), "%s/%s", dir, argv[1]);
        if (access(full, X_OK) == 0) {
            printf("%s is %s\n", argv[1], full);
            return 0;
        }
        dir = strtok(NULL, ":");
    }

    printf("%s: not found\n", argv[1]);
    return 1;
}
