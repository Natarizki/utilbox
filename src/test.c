/* test.c — evaluate a conditional expression, exit 0 if true, 1 if false.
 * Supports: -f FILE (regular file), -d FILE (directory), -e FILE (exists),
 * -z STR (empty string), -n STR (non-empty string),
 * STR1 = STR2, STR1 != STR2, N1 -eq N2, N1 -ne N2, N1 -lt N2, N1 -gt N2. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "utilbox.h"

int test_main(int argc, char **argv)
{
    if (argc == 2) {
        return (argv[1][0] != '\0') ? 0 : 1;
    }

    if (argc == 3) {
        const char *op = argv[1];
        const char *arg = argv[2];
        struct stat st;

        if (strcmp(op, "-f") == 0) {
            return (stat(arg, &st) == 0 && S_ISREG(st.st_mode)) ? 0 : 1;
        }
        if (strcmp(op, "-d") == 0) {
            return (stat(arg, &st) == 0 && S_ISDIR(st.st_mode)) ? 0 : 1;
        }
        if (strcmp(op, "-e") == 0) {
            return (stat(arg, &st) == 0) ? 0 : 1;
        }
        if (strcmp(op, "-z") == 0) {
            return (strlen(arg) == 0) ? 0 : 1;
        }
        if (strcmp(op, "-n") == 0) {
            return (strlen(arg) > 0) ? 0 : 1;
        }
    }

    if (argc == 4) {
        const char *a = argv[1];
        const char *op = argv[2];
        const char *b = argv[3];

        if (strcmp(op, "=") == 0) return (strcmp(a, b) == 0) ? 0 : 1;
        if (strcmp(op, "!=") == 0) return (strcmp(a, b) != 0) ? 0 : 1;

        long na = atol(a), nb = atol(b);
        if (strcmp(op, "-eq") == 0) return (na == nb) ? 0 : 1;
        if (strcmp(op, "-ne") == 0) return (na != nb) ? 0 : 1;
        if (strcmp(op, "-lt") == 0) return (na < nb) ? 0 : 1;
        if (strcmp(op, "-gt") == 0) return (na > nb) ? 0 : 1;
        if (strcmp(op, "-le") == 0) return (na <= nb) ? 0 : 1;
        if (strcmp(op, "-ge") == 0) return (na >= nb) ? 0 : 1;
    }

    return fail("test", "unsupported expression");
}
