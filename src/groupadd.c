/* groupadd.c — append a new group entry to /etc/group */
#include <stdio.h>
#include "utilbox.h"

int groupadd_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("groupadd", "usage: groupadd GROUPNAME");
    }

    FILE *f = fopen("/etc/group", "r");
    int next_gid = 1000;
    if (f) {
        char line[512];
        while (fgets(line, sizeof(line), f) != NULL) {
            int gid;
            if (sscanf(line, "%*[^:]:%*[^:]:%d:", &gid) == 1) {
                if (gid >= next_gid) next_gid = gid + 1;
            }
        }
        fclose(f);
    }

    FILE *out = fopen("/etc/group", "a");
    if (!out) return die("groupadd", "cannot open /etc/group (need privilege)", NULL);

    fprintf(out, "%s:x:%d:\n", argv[1], next_gid);
    fclose(out);

    return 0;
}
