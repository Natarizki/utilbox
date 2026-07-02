/* tput.c — simplified terminal capability query.
 * Supports: cols, lines, clear, reset (via ANSI escapes / ioctl). */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "utilbox.h"

int tput_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("tput", "usage: tput cols|lines|clear|reset");
    }

    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    if (strcmp(argv[1], "cols") == 0) {
        printf("%d\n", ws.ws_col);
    }
    else if (strcmp(argv[1], "lines") == 0) {
        printf("%d\n", ws.ws_row);
    }
    else if (strcmp(argv[1], "clear") == 0) {
        fputs("\033[H\033[2J", stdout);
    }
    else if (strcmp(argv[1], "reset") == 0) {
        fputs("\033c", stdout);
    }
    else {
        return fail("tput", "unknown capability");
    }

    return 0;
}
