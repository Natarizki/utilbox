/* stty.c — simplified terminal settings: print size, or toggle echo mode. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "utilbox.h"

int stty_main(int argc, char **argv)
{
    if (argc < 2) {
        struct winsize ws;
        ioctl(0, TIOCGWINSZ, &ws);
        printf("rows %d; columns %d;\n", ws.ws_row, ws.ws_col);
        return 0;
    }

    struct termios tio;
    if (tcgetattr(0, &tio) != 0) {
        return die("stty", "not a terminal", NULL);
    }

    if (strcmp(argv[1], "echo") == 0) {
        tio.c_lflag |= ECHO;
    }
    else if (strcmp(argv[1], "-echo") == 0) {
        tio.c_lflag &= ~ECHO;
    }
    else {
        return fail("stty", "usage: stty [echo|-echo]");
    }

    tcsetattr(0, TCSANOW, &tio);
    return 0;
}
