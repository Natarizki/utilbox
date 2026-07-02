/* chsh.c — change a user's login shell in /etc/passwd.
 * Usage: chsh USERNAME NEWSHELL */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int chsh_main(int argc, char **argv)
{
    if (argc != 3) {
        return fail("chsh", "usage: chsh USERNAME NEWSHELL");
    }

    FILE *in = fopen("/etc/passwd", "r");
    if (!in) return die("chsh", "cannot open /etc/passwd", NULL);

    FILE *out = fopen("/etc/passwd.tmp", "w");
    if (!out) {
        fclose(in);
        return die("chsh", "cannot create temp file (need privilege)", NULL);
    }

    char line[512];
    size_t namelen = strlen(argv[1]);
    int found = 0;

    while (fgets(line, sizeof(line), in) != NULL) {
        if (strncmp(line, argv[1], namelen) == 0 && line[namelen] == ':') {
            found = 1;
            char fields[6][256];
            char *tok = strtok(line, ":");
            int i = 0;
            while (tok != NULL && i < 6) {
                strncpy(fields[i], tok, sizeof(fields[i]) - 1);
                fields[i][sizeof(fields[i])-1] = '\0';
                tok = strtok(NULL, ":");
                i++;
            }
            fprintf(out, "%s:%s:%s:%s:%s:%s:%s\n",
                    fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], argv[2]);
        } else {
            fputs(line, out);
        }
    }

    fclose(in);
    fclose(out);
    rename("/etc/passwd.tmp", "/etc/passwd");

    if (!found) {
        fprintf(stderr, "utilbox: chsh: user '%s' not found\n", argv[1]);
        return 1;
    }

    return 0;
}
