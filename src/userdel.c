/* userdel.c — remove a user entry from /etc/passwd */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilbox.h"

int userdel_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("userdel", "usage: userdel USERNAME");
    }

    FILE *in = fopen("/etc/passwd", "r");
    if (!in) return die("userdel", "cannot open /etc/passwd", NULL);

    FILE *out = fopen("/etc/passwd.tmp", "w");
    if (!out) {
        fclose(in);
        return die("userdel", "cannot create temp file (need privilege)", NULL);
    }

    char line[512];
    size_t namelen = strlen(argv[1]);
    int found = 0;

    while (fgets(line, sizeof(line), in) != NULL) {
        if (strncmp(line, argv[1], namelen) == 0 && line[namelen] == ':') {
            found = 1;
            continue;
        }
        fputs(line, out);
    }

    fclose(in);
    fclose(out);
    rename("/etc/passwd.tmp", "/etc/passwd");

    if (!found) {
        fprintf(stderr, "utilbox: userdel: user '%s' not found\n", argv[1]);
        return 1;
    }

    return 0;
}
