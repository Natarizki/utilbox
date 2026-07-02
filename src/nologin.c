/* nologin.c — print a message and refuse login; used as a shell for
 * disabled/system accounts in /etc/passwd. */
#include <stdio.h>

int nologin_main(int argc, char **argv)
{
    (void)argc; (void)argv;
    printf("This account is currently not available.\n");
    return 1;
}
