/* setsid.c — run a command in a new session (detached from controlling terminal) */
#include <unistd.h>
#include "utilbox.h"

int setsid_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("setsid", "usage: setsid COMMAND [ARGS...]");
    }

    if (setsid() == (pid_t)-1) {
        fprintf(stderr, "utilbox: setsid: warning: already a session leader\n");
    }

    execvp(argv[1], argv + 1);
    return die("setsid", "cannot execute", argv[1]);
}
