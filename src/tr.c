/* tr.c — translate or delete characters from stdin.
 * Usage: tr SET1 SET2       (translate chars in SET1 to corresponding in SET2)
 *        tr -d SET1         (delete chars in SET1)
 * Sets are literal character lists (no ranges like a-z in v1). */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

int tr_main(int argc, char **argv)
{
    int delete_mode = 0;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-d") == 0) {
        delete_mode = 1;
        i++;
    }

    if (delete_mode) {
        if (i >= argc) return fail("tr", "usage: tr -d SET1");
        const char *set1 = argv[i];

        int c;
        while ((c = fgetc(stdin)) != EOF) {
            if (strchr(set1, c) == NULL) {
                putchar(c);
            }
        }
        return 0;
    }

    if (i + 2 > argc) {
        return fail("tr", "usage: tr SET1 SET2");
    }

    const char *set1 = argv[i];
    const char *set2 = argv[i + 1];
    size_t len1 = strlen(set1);
    size_t len2 = strlen(set2);

    int c;
    while ((c = fgetc(stdin)) != EOF) {
        const char *pos = strchr(set1, c);
        if (pos != NULL && len2 > 0) {
            size_t idx = pos - set1;
            if (idx >= len2) idx = len2 - 1;
            putchar(set2[idx]);
        } else {
            putchar(c);
        }
    }

    return 0;
}
