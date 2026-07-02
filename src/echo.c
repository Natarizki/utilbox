/* echo.c — print arguments to stdout, separated by spaces, followed by newline.
 * Supports -n (no trailing newline) like most shells' builtin echo. */
#include <stdio.h>
#include <string.h>

int echo_main(int argc, char **argv)
{
    int i = 1;
    int newline = 1;

    if (i < argc && strcmp(argv[i], "-n") == 0) {
        newline = 0;
        i++;
    }

    for (; i < argc; i++) {
        fputs(argv[i], stdout);
        if (i + 1 < argc) fputc(' ', stdout);
    }

    if (newline) fputc('\n', stdout);

    return 0;
}
