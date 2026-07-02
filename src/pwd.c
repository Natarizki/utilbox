/* pwd.c — print current working directory */
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "utilbox.h"

int pwd_main(int argc, char **argv)
{
    (void)argc;
    char buf[PATH_MAX];

    if (getcwd(buf, sizeof(buf)) == NULL) {
        return die(argv[0], "getcwd failed", NULL);
    }

    puts(buf);
    return 0;
}
