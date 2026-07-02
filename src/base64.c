/* base64.c — base64 encode/decode. Usage: base64 [-d] [FILE] */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

static const char b64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void b64_encode_stream(FILE *in)
{
    unsigned char buf[3];
    int col = 0;
    size_t n;

    while ((n = fread(buf, 1, 3, in)) > 0) {
        unsigned char b0 = buf[0], b1 = (n > 1) ? buf[1] : 0, b2 = (n > 2) ? buf[2] : 0;

        putchar(b64_table[b0 >> 2]);
        putchar(b64_table[((b0 & 0x03) << 4) | (b1 >> 4)]);
        putchar(n > 1 ? b64_table[((b1 & 0x0F) << 2) | (b2 >> 6)] : '=');
        putchar(n > 2 ? b64_table[b2 & 0x3F] : '=');

        col += 4;
        if (col >= 76) { putchar('\n'); col = 0; }
    }
    if (col > 0) putchar('\n');
}

static int b64_val(int c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

static void b64_decode_stream(FILE *in)
{
    int vals[4];
    int nv = 0;
    int c;

    while ((c = fgetc(in)) != EOF) {
        if (c == '=' || c == '\n' || c == '\r') continue;
        int v = b64_val(c);
        if (v < 0) continue;
        vals[nv++] = v;

        if (nv == 4) {
            putchar((vals[0] << 2) | (vals[1] >> 4));
            putchar(((vals[1] & 0x0F) << 4) | (vals[2] >> 2));
            putchar(((vals[2] & 0x03) << 6) | vals[3]);
            nv = 0;
        }
    }

    if (nv == 3) {
        putchar((vals[0] << 2) | (vals[1] >> 4));
        putchar(((vals[1] & 0x0F) << 4) | (vals[2] >> 2));
    } else if (nv == 2) {
        putchar((vals[0] << 2) | (vals[1] >> 4));
    }
}

int base64_main(int argc, char **argv)
{
    int decode = 0;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-d") == 0) {
        decode = 1;
        i++;
    }

    FILE *in = stdin;
    if (i < argc) {
        in = fopen(argv[i], "rb");
        if (!in) return die("base64", "cannot open", argv[i]);
    }

    if (decode) b64_decode_stream(in);
    else b64_encode_stream(in);

    if (in != stdin) fclose(in);
    return 0;
}
