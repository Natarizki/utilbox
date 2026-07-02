/* factor.c — print the prime factorization of a number */
#include <stdio.h>
#include <stdlib.h>
#include "utilbox.h"

int factor_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("factor", "usage: factor NUMBER");
    }

    long n = atol(argv[1]);
    printf("%ld:", n);

    if (n < 2) {
        printf("\n");
        return 0;
    }

    for (long p = 2; p * p <= n; p++) {
        while (n % p == 0) {
            printf(" %ld", p);
            n /= p;
        }
    }
    if (n > 1) {
        printf(" %ld", n);
    }
    printf("\n");

    return 0;
}
