/* yes.c — repeatedly print a string (default "y") until interrupted */
#include <stdio.h>

int yes_main(int argc, char **argv)
{
    const char *msg = (argc >= 2) ? argv[1] : "y";
    for (;;) {
        puts(msg);
    }
    return 0;
}
