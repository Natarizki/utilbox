/* join.c — join lines of two sorted files on their first whitespace-separated
 * field. Simplified: assumes both files are sorted by the join field. */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

static void split_first_field(char *line, char *key, size_t keysz, char **rest)
{
    char *sp = strchr(line, ' ');
    if (!sp) sp = strchr(line, '\t');

    if (sp) {
        size_t klen = sp - line;
        if (klen >= keysz) klen = keysz - 1;
        memcpy(key, line, klen);
        key[klen] = '\0';
        *rest = sp + 1;
    } else {
        strncpy(key, line, keysz - 1);
        key[keysz - 1] = '\0';
        *rest = line + strlen(line);
    }
}

int join_main(int argc, char **argv)
{
    if (argc != 3) {
        return fail("join", "usage: join FILE1 FILE2");
    }

    FILE *f1 = fopen(argv[1], "r");
    if (!f1) return die("join", "cannot open", argv[1]);
    FILE *f2 = fopen(argv[2], "r");
    if (!f2) { fclose(f1); return die("join", "cannot open", argv[2]); }

    char l1[4096], l2[4096];
    char k1[256], k2[256];
    char *r1, *r2;

    int have1 = (fgets(l1, sizeof(l1), f1) != NULL);
    int have2 = (fgets(l2, sizeof(l2), f2) != NULL);
    if (have1) split_first_field(l1, k1, sizeof(k1), &r1);
    if (have2) split_first_field(l2, k2, sizeof(k2), &r2);

    while (have1 && have2) {
        int cmp = strcmp(k1, k2);
        if (cmp < 0) {
            have1 = (fgets(l1, sizeof(l1), f1) != NULL);
            if (have1) split_first_field(l1, k1, sizeof(k1), &r1);
        }
        else if (cmp > 0) {
            have2 = (fgets(l2, sizeof(l2), f2) != NULL);
            if (have2) split_first_field(l2, k2, sizeof(k2), &r2);
        }
        else {
            size_t r1len = strlen(r1);
            if (r1len > 0 && r1[r1len - 1] == '\n') r1[r1len - 1] = '\0';
            printf("%s %s %s", k1, r1, r2);

            have1 = (fgets(l1, sizeof(l1), f1) != NULL);
            if (have1) split_first_field(l1, k1, sizeof(k1), &r1);
            have2 = (fgets(l2, sizeof(l2), f2) != NULL);
            if (have2) split_first_field(l2, k2, sizeof(k2), &r2);
        }
    }

    fclose(f1);
    fclose(f2);
    return 0;
}
