/* arch.c — print machine architecture (same as `uname -m`) */
#include <stdio.h>
#include <sys/utsname.h>
#include "utilbox.h"

int arch_main(int argc, char **argv)
{
    (void)argc; (void)argv;
    struct utsname u;
    if (uname(&u) != 0) {
        return die("arch", "uname() failed", NULL);
    }
    puts(u.machine);
    return 0;
}
