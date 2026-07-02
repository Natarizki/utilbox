/* uname.c — print system information. Supports -a (all), -s (kernel name),
 * -r (kernel release), -m (machine). Default (no args): -s. */
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include "utilbox.h"

int uname_main(int argc, char **argv)
{
    struct utsname u;
    if (uname(&u) != 0) {
        return die("uname", "uname() failed", NULL);
    }

    if (argc < 2) {
        puts(u.sysname);
        return 0;
    }

    if (strcmp(argv[1], "-a") == 0) {
        printf("%s %s %s %s %s\n", u.sysname, u.nodename, u.release, u.version, u.machine);
    }
    else if (strcmp(argv[1], "-s") == 0) {
        puts(u.sysname);
    }
    else if (strcmp(argv[1], "-r") == 0) {
        puts(u.release);
    }
    else if (strcmp(argv[1], "-m") == 0) {
        puts(u.machine);
    }
    else if (strcmp(argv[1], "-n") == 0) {
        puts(u.nodename);
    }
    else {
        return fail("uname", "usage: uname [-a|-s|-r|-m|-n]");
    }

    return 0;
}
