/* diff.c — compare two files line by line using a simple LCS algorithm,
 * printing classic diff-style output (< / > / ---). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilbox.h"

static char **read_lines(const char *path, size_t *count)
{
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    char **lines = NULL;
    size_t cap = 0, n = 0;
    char buf[65536];

    while (fgets(buf, sizeof(buf), f) != NULL) {
        if (n == cap) {
            cap = cap ? cap * 2 : 64;
            lines = realloc(lines, cap * sizeof(char *));
        }
        lines[n++] = strdup(buf);
    }
    fclose(f);
    *count = n;
    return lines;
}

int diff_main(int argc, char **argv)
{
    if (argc != 3) {
        return fail("diff", "usage: diff FILE1 FILE2");
    }

    size_t n1, n2;
    char **l1 = read_lines(argv[1], &n1);
    char **l2 = read_lines(argv[2], &n2);
    if (!l1) return die("diff", "cannot open", argv[1]);
    if (!l2) return die("diff", "cannot open", argv[2]);

    size_t **lcs = malloc((n1 + 1) * sizeof(size_t *));
    for (size_t i = 0; i <= n1; i++) {
        lcs[i] = calloc(n2 + 1, sizeof(size_t));
    }

    for (size_t i = 1; i <= n1; i++) {
        for (size_t j = 1; j <= n2; j++) {
            if (strcmp(l1[i-1], l2[j-1]) == 0) {
                lcs[i][j] = lcs[i-1][j-1] + 1;
            } else {
                lcs[i][j] = (lcs[i-1][j] > lcs[i][j-1]) ? lcs[i-1][j] : lcs[i][j-1];
            }
        }
    }

    int differ = 0;
    size_t ii = 0, jj = 0;
    while (ii < n1 || jj < n2) {
        if (ii < n1 && jj < n2 && strcmp(l1[ii], l2[jj]) == 0) {
            ii++; jj++;
        }
        else if (jj < n2 && (ii == n1 || lcs[ii][jj+1] >= lcs[ii+1][jj])) {
            printf("> %s", l2[jj]);
            if (l2[jj][strlen(l2[jj])-1] != '\n') printf("\n");
            differ = 1;
            jj++;
        }
        else {
            printf("< %s", l1[ii]);
            if (l1[ii][strlen(l1[ii])-1] != '\n') printf("\n");
            differ = 1;
            ii++;
        }
    }

    for (size_t k = 0; k <= n1; k++) free(lcs[k]);
    free(lcs);
    for (size_t k = 0; k < n1; k++) free(l1[k]);
    for (size_t k = 0; k < n2; k++) free(l2[k]);
    free(l1); free(l2);

    return differ ? 1 : 0;
}
