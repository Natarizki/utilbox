/* write.c — send a message to a specific terminal device.
 * Usage: write TTY MESSAGE */
#include <stdio.h>
#include "utilbox.h"

int write_main(int argc, char **argv)
{
    if (argc < 3) {
        return fail("write", "usage: write TTY MESSAGE");
    }

    FILE *f = fopen(argv[1], "w");
    if (!f) return die("write", "cannot open", argv[1]);

    fprintf(f, "Message: %s\n", argv[2]);
    fclose(f);

    return 0;
}
