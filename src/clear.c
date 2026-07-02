/* clear.c — clear the terminal screen using the standard ANSI escape sequence */
#include <stdio.h>

int clear_main(int argc, char **argv)
{
    (void)argc; (void)argv;
    fputs("\033[H\033[2J", stdout);
    fflush(stdout);
    return 0;
}
