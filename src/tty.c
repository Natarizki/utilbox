/* tty.c — print the filename of the terminal connected to stdin */
#include <stdio.h>
#include <unistd.h>
#include "utilbox.h"

int tty_main(int argc, char **argv)
{
    (void)argc; (void)argv;
    char *name = ttyname(0);
    if (!name) {
        puts("not a tty");
        return 1;
    }
    puts(name);
    return 0;
}
