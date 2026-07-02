/* strings.c — extract printable character sequences from a (binary) file.
 * Minimum sequence length defaults to 4. */
#include <stdio.h>
#include <ctype.h>
#include "utilbox.h"

static void strings_stream(FILE *in, int minlen)
{
    char buf[65536];
    int len = 0;
    int c;

    while ((c = fgetc(in)) != EOF) {
        if (isprint((unsigned char)c) || c == '\t') {
            if (len < (int)sizeof(buf) - 1) {
                buf[len++] = (char)c;
            }
        } else {
            if (len >= minlen) {
                buf[len] = '\0';
                puts(buf);
            }
            len = 0;
        }
    }
    if (len >= minlen) {
        buf[len] = '\0';
        puts(buf);
    }
}

int strings_main(int argc, char **argv)
{
    int minlen = 4;

    if (argc < 2) {
        strings_stream(stdin, minlen);
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "rb");
        if (!f) {
            rc |= die("strings", "cannot open", argv[i]);
            continue;
        }
        strings_stream(f, minlen);
        fclose(f);
    }

    return rc;
}
