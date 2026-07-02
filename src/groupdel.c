/* groupdel.c — remove a group entry from /etc/group */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilbox.h"

int groupdel_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("groupdel", "usage: groupdel GROUPNAME");
    }

    FILE *in = fopen("/etc/group", "r");
    if (!in) return die("groupdel", "cannot open /etc/group", NULL);

    FILE *out = fopen("/etc/group.tmp", "w");
    if (!out) {
        fclose(in);
        return die("groupdel", "cannot create temp file (need privilege)", NULL);
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
    rename("/etc/group.tmp", "/etc/group");

    if (!found) {
        fprintf(stderr, "utilbox: groupdel: group '%s' not found\n", argv[1]);
        return 1;
    }

    return 0;
}
