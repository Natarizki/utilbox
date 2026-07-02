/* expr.c — evaluate a simple arithmetic/string expression.
 * Usage: expr A + B | A - B | A \* B | A / B | A = B */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilbox.h"

int expr_main(int argc, char **argv)
{
    if (argc != 4) {
        return fail("expr", "usage: expr A OP B");
    }

    const char *op = argv[2];

    if (strcmp(op, "=") == 0) {
        int eq = (strcmp(argv[1], argv[3]) == 0);
        printf("%d\n", eq);
        return eq ? 0 : 1;
    }

    long a = atol(argv[1]);
    long b = atol(argv[3]);
    long result;

    if (strcmp(op, "+") == 0) result = a + b;
    else if (strcmp(op, "-") == 0) result = a - b;
    else if (strcmp(op, "*") == 0) result = a * b;
    else if (strcmp(op, "/") == 0) {
        if (b == 0) return fail("expr", "division by zero");
        result = a / b;
    }
    else if (strcmp(op, "%") == 0) {
        if (b == 0) return fail("expr", "division by zero");
        result = a % b;
    }
    else {
        return fail("expr", "unsupported operator");
    }

    printf("%ld\n", result);
    return (result != 0) ? 0 : 1;
}
