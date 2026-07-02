/* sum.c — simple checksum (BSD-style 16-bit rotating checksum) and block count */
#include <stdio.h>
#include "utilbox.h"

static void sum_stream(FILE *in, unsigned long *checksum, long *blocks)
{
    unsigned long sum = 0;
    long total = 0;
    int c;

    while ((c = fgetc(in)) != EOF) {
        sum = (sum >> 1) + ((sum & 1) << 15);
        sum += c;
        sum &= 0xFFFF;
        total++;
    }

    *checksum = sum;
    *blocks = (total + 1023) / 1024;
}

int sum_main(int argc, char **argv)
{
    if (argc < 2) {
        unsigned long sum; long blocks;
        sum_stream(stdin, &sum, &blocks);
        printf("%05lu %5ld\n", sum, blocks);
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "rb");
        if (!f) {
            rc |= die("sum", "cannot open", argv[i]);
            continue;
        }
        unsigned long sum; long blocks;
        sum_stream(f, &sum, &blocks);
        fclose(f);
        printf("%05lu %5ld %s\n", sum, blocks, argv[i]);
    }

    return rc;
}
