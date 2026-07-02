/* useradd.c — append a new user entry to /etc/passwd (simplified: no
 * shadow password hashing, no home directory creation). */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int useradd_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("useradd", "usage: useradd USERNAME");
    }

    FILE *f = fopen("/etc/passwd", "r");
    int next_uid = 1000;
    if (f) {
        char line[512];
        while (fgets(line, sizeof(line), f) != NULL) {
            int uid;
            if (sscanf(line, "%*[^:]:%*[^:]:%d:", &uid) == 1) {
                if (uid >= next_uid) next_uid = uid + 1;
            }
        }
        fclose(f);
    }

    FILE *out = fopen("/etc/passwd", "a");
    if (!out) return die("useradd", "cannot open /etc/passwd (need privilege)", NULL);

    fprintf(out, "%s:x:%d:%d::/home/%s:/bin/sh\n", argv[1], next_uid, next_uid, argv[1]);
    fclose(out);

    return 0;
}
