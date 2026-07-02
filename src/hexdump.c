/* hexdump.c — canonical hex+ASCII dump (similar to `hexdump -C`) */
#include <stdio.h>
#include <ctype.h>
#include "utilbox.h"

static void hexdump_stream(FILE *in)
{
    unsigned char buf[16];
    size_t n;
    long offset = 0;

    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        printf("%08lx  ", offset);
        for (size_t i = 0; i < 16; i++) {
            if (i < n) printf("%02x ", buf[i]);
            else printf("   ");
            if (i == 7) printf(" ");
        }
        printf(" |");
        for (size_t i = 0; i < n; i++) {
            putchar(isprint(buf[i]) ? buf[i] : '.');
        }
        printf("|\n");
        offset += n;
    }
    printf("%08lx\n", offset);
}

int hexdump_main(int argc, char **argv)
{
    if (argc < 2) {
        hexdump_stream(stdin);
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "rb");
        if (!f) {
            rc |= die("hexdump", "cannot open", argv[i]);
            continue;
        }
        hexdump_stream(f);
        fclose(f);
    }

    return rc;
}
