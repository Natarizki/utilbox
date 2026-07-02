/* sha1sum.c — SHA-1 message digest, implemented from scratch per FIPS 180-1. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "utilbox.h"

typedef struct {
    uint32_t state[5];
    uint64_t count;
    unsigned char buffer[64];
    size_t buflen;
} sha1_ctx;

static uint32_t rotl(uint32_t x, int c) { return (x << c) | (x >> (32 - c)); }

static void sha1_init(sha1_ctx *ctx)
{
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
    ctx->count = 0;
    ctx->buflen = 0;
}

static void sha1_process_block(sha1_ctx *ctx, const unsigned char *block)
{
    uint32_t w[80];
    for (int i = 0; i < 16; i++) {
        w[i] = ((uint32_t)block[i*4] << 24) | ((uint32_t)block[i*4+1] << 16) |
               ((uint32_t)block[i*4+2] << 8) | (uint32_t)block[i*4+3];
    }
    for (int i = 16; i < 80; i++) {
        w[i] = rotl(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
    }

    uint32_t a = ctx->state[0], b = ctx->state[1], c = ctx->state[2],
             d = ctx->state[3], e = ctx->state[4];

    for (int i = 0; i < 80; i++) {
        uint32_t f, k;
        if (i < 20) { f = (b & c) | (~b & d); k = 0x5A827999; }
        else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
        else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
        else { f = b ^ c ^ d; k = 0xCA62C1D6; }

        uint32_t temp = rotl(a, 5) + f + e + k + w[i];
        e = d; d = c; c = rotl(b, 30); b = a; a = temp;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
}

static void sha1_update(sha1_ctx *ctx, const unsigned char *data, size_t len)
{
    ctx->count += (uint64_t)len * 8;

    while (len > 0) {
        size_t take = 64 - ctx->buflen;
        if (take > len) take = len;
        memcpy(ctx->buffer + ctx->buflen, data, take);
        ctx->buflen += take;
        data += take;
        len -= take;

        if (ctx->buflen == 64) {
            sha1_process_block(ctx, ctx->buffer);
            ctx->buflen = 0;
        }
    }
}

static void sha1_final(sha1_ctx *ctx, unsigned char digest[20])
{
    uint64_t bitcount = ctx->count;
    unsigned char pad = 0x80;
    sha1_update(ctx, &pad, 1);

    unsigned char zero = 0;
    while (ctx->buflen != 56) {
        sha1_update(ctx, &zero, 1);
    }

    unsigned char lenbytes[8];
    for (int i = 0; i < 8; i++) {
        lenbytes[7 - i] = (unsigned char)(bitcount >> (8 * i));
    }
    memcpy(ctx->buffer + ctx->buflen, lenbytes, 8);
    sha1_process_block(ctx, ctx->buffer);

    for (int i = 0; i < 5; i++) {
        digest[i*4]   = (unsigned char)(ctx->state[i] >> 24);
        digest[i*4+1] = (unsigned char)(ctx->state[i] >> 16);
        digest[i*4+2] = (unsigned char)(ctx->state[i] >> 8);
        digest[i*4+3] = (unsigned char)(ctx->state[i]);
    }
}

static void sha1_file(FILE *f, unsigned char digest[20])
{
    sha1_ctx ctx;
    sha1_init(&ctx);

    unsigned char buf[8192];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        sha1_update(&ctx, buf, n);
    }
    sha1_final(&ctx, digest);
}

int sha1sum_main(int argc, char **argv)
{
    unsigned char digest[20];

    if (argc < 2) {
        sha1_file(stdin, digest);
        for (int i = 0; i < 20; i++) printf("%02x", digest[i]);
        printf("  -\n");
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "rb");
        if (!f) {
            rc |= die("sha1sum", "cannot open", argv[i]);
            continue;
        }
        sha1_file(f, digest);
        fclose(f);
        for (int j = 0; j < 20; j++) printf("%02x", digest[j]);
        printf("  %s\n", argv[i]);
    }

    return rc;
}
