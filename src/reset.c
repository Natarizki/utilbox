/* reset.c — fully reset the terminal (more thorough than `clear`):
 * sends the terminal reset escape sequence, restoring default colors,
 * cursor state, and clearing scrollback-visible area. */
#include <stdio.h>

int reset_main(int argc, char **argv)
{
    (void)argc; (void)argv;
    fputs("\033c\033[3J\033[H\033[2J", stdout);
    fflush(stdout);
    return 0;
}
