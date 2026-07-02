/* whoami.c — print the current effective username */
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include "utilbox.h"

int whoami_main(int argc, char **argv)
{
    (void)argc;
    struct passwd *pw = getpwuid(geteuid());
    if (!pw) {
        return die(argv[0], "cannot determine username", NULL);
    }
    puts(pw->pw_name);
    return 0;
}
