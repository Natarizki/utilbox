/* od.c — dump file contents in hex (simplified: hex only, not full od's
 * octal/decimal format options). Output format similar to `od -A x -t x1`. */
#include <stdio.h>
#include "utilbox.h"

static void od_stream(FILE *in)
{
    unsigned char buf[16];
    size_t n;
    long offset = 0;

    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        printf("%07lo ", offset);
        for (size_t i = 0; i < 16; i++) {
            if (i < n) printf("%02x ", buf[i]);
            else printf("   ");
            if (i == 7) printf(" ");
        }
        printf("\n");
        offset += n;
    }
    printf("%07lo\n", offset);
}

int od_main(int argc, char **argv)
{
    if (argc < 2) {
        od_stream(stdin);
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "rb");
        if (!f) {
            rc |= die("od", "cannot open", argv[i]);
            continue;
        }
        od_stream(f);
        fclose(f);
    }

    return rc;
}
