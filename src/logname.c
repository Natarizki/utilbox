/* logname.c — print the login name of the current user */
#include <stdio.h>
#include <unistd.h>
#include "utilbox.h"

int logname_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    char *name = getlogin();
    if (!name) {
        return die("logname", "cannot determine login name", NULL);
    }

    puts(name);
    return 0;
}
