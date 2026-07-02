/* printenv.c — print environment variables. With an argument, print only
 * that variable's value; without one, print all (NAME=value pairs). */
#include <stdio.h>
#include <stdlib.h>
#include "utilbox.h"

extern char **environ;

int printenv_main(int argc, char **argv)
{
    if (argc >= 2) {
        const char *val = getenv(argv[1]);
        if (!val) return 1;
        puts(val);
        return 0;
    }

    for (char **e = environ; *e != NULL; e++) {
        puts(*e);
    }
    return 0;
}
