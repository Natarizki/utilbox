/* env.c — print environment variables, or run a command with the current
 * environment (extern char **environ is used directly). */
#include <stdio.h>
#include <unistd.h>
#include "utilbox.h"

extern char **environ;

int env_main(int argc, char **argv)
{
    if (argc < 2) {
        for (char **e = environ; *e != NULL; e++) {
            puts(*e);
        }
        return 0;
    }

    execvp(argv[1], argv + 1);
    return die("env", "cannot execute", argv[1]);
}
