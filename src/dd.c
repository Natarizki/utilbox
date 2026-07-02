/* dd.c — copy data with configurable block size and count.
 * Usage: dd if=FILE of=FILE [bs=N] [count=N]  (simplified: no seek/skip) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilbox.h"

int dd_main(int argc, char **argv)
{
    const char *infile = NULL, *outfile = NULL;
    long bs = 512;
    long count = -1;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "if=", 3) == 0) infile = argv[i] + 3;
        else if (strncmp(argv[i], "of=", 3) == 0) outfile = argv[i] + 3;
        else if (strncmp(argv[i], "bs=", 3) == 0) bs = atol(argv[i] + 3);
        else if (strncmp(argv[i], "count=", 6) == 0) count = atol(argv[i] + 6);
    }

    FILE *in = infile ? fopen(infile, "rb") : stdin;
    if (!in) return die("dd", "cannot open input", infile);

    FILE *out = outfile ? fopen(outfile, "wb") : stdout;
    if (!out) {
        if (in != stdin) fclose(in);
        return die("dd", "cannot open output", outfile);
    }

    char *buf = malloc(bs);
    long blocks = 0;
    size_t n;

    while ((count < 0 || blocks < count) && (n = fread(buf, 1, bs, in)) > 0) {
        fwrite(buf, 1, n, out);
        blocks++;
    }

    free(buf);
    if (in != stdin) fclose(in);
    if (out != stdout) fclose(out);

    fprintf(stderr, "%ld+0 records in\n%ld+0 records out\n", blocks, blocks);
    return 0;
}
