/* hostname.c — print (or set, if given an argument) the system hostname */
#include <stdio.h>
#include <unistd.h>
#include "utilbox.h"

int hostname_main(int argc, char **argv)
{
    if (argc >= 2) {
        if (sethostname(argv[1], strlen(argv[1])) != 0) {
            return die("hostname", "cannot set hostname", argv[1]);
        }
        return 0;
    }

    char buf[256];
    if (gethostname(buf, sizeof(buf)) != 0) {
        return die("hostname", "cannot get hostname", NULL);
    }
    puts(buf);
    return 0;
}
