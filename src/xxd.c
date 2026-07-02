/* xxd.c — hex dump utility. Supports -r (reverse: hex dump back to binary). */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utilbox.h"

static void xxd_dump(FILE *in)
{
    unsigned char buf[16];
    size_t n;
    long offset = 0;

    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        printf("%08lx: ", offset);
        for (size_t i = 0; i < 16; i++) {
            if (i < n) printf("%02x", buf[i]);
            else printf("  ");
            if (i % 2 == 1) printf(" ");
        }
        printf(" ");
        for (size_t i = 0; i < n; i++) {
            putchar(isprint(buf[i]) ? buf[i] : '.');
        }
        printf("\n");
        offset += n;
    }
}

static void xxd_reverse(FILE *in)
{
    char line[256];
    while (fgets(line, sizeof(line), in) != NULL) {
        char *colon = strchr(line, ':');
        char *p = colon ? colon + 1 : line;

        while (*p) {
            while (*p == ' ') p++;
            if (!isxdigit((unsigned char)*p) || !isxdigit((unsigned char)*(p+1))) break;
            unsigned byte;
            sscanf(p, "%2x", &byte);
            putchar(byte);
            p += 2;
        }
    }
}

int xxd_main(int argc, char **argv)
{
    int reverse = 0;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-r") == 0) {
        reverse = 1;
        i++;
    }

    FILE *in = stdin;
    if (i < argc) {
        in = fopen(argv[i], "rb");
        if (!in) return die("xxd", "cannot open", argv[i]);
    }

    if (reverse) xxd_reverse(in);
    else xxd_dump(in);

    if (in != stdin) fclose(in);
    return 0;
}
