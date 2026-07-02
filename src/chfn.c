/* chfn.c — change a user's full name (GECOS field) in /etc/passwd.
 * Usage: chfn USERNAME "Full Name" */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int chfn_main(int argc, char **argv)
{
    if (argc != 3) {
        return fail("chfn", "usage: chfn USERNAME \"Full Name\"");
    }

    FILE *in = fopen("/etc/passwd", "r");
    if (!in) return die("chfn", "cannot open /etc/passwd", NULL);

    FILE *out = fopen("/etc/passwd.tmp", "w");
    if (!out) {
        fclose(in);
        return die("chfn", "cannot create temp file (need privilege)", NULL);
    }

    char line[512];
    size_t namelen = strlen(argv[1]);
    int found = 0;

    while (fgets(line, sizeof(line), in) != NULL) {
        if (strncmp(line, argv[1], namelen) == 0 && line[namelen] == ':') {
            found = 1;
            char *parts[7];
            char *tok = strtok(line, ":\n");
            int i = 0;
            while (tok != NULL && i < 7) {
                parts[i++] = tok;
                tok = strtok(NULL, ":\n");
            }
            fprintf(out, "%s:%s:%s:%s:%s:%s:%s\n",
                    parts[0], parts[1], parts[2], parts[3], argv[2],
                    i > 5 ? parts[5] : "", i > 6 ? parts[6] : "/bin/sh");
        } else {
            fputs(line, out);
        }
    }

    fclose(in);
    fclose(out);
    rename("/etc/passwd.tmp", "/etc/passwd");

    if (!found) {
        fprintf(stderr, "utilbox: chfn: user '%s' not found\n", argv[1]);
        return 1;
    }

    return 0;
}
