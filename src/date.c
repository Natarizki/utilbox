/* date.c — print the current date and time. Supports +FORMAT (strftime-style). */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "utilbox.h"

int date_main(int argc, char **argv)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char buf[256];

    if (argc >= 2 && argv[1][0] == '+') {
        strftime(buf, sizeof(buf), argv[1] + 1, tm_info);
    } else {
        strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Z %Y", tm_info);
    }

    puts(buf);
    return 0;
}
