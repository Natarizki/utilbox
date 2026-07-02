/* md5sum.c — MD5 message digest, implemented from scratch per RFC 1321. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "utilbox.h"

typedef struct {
    uint32_t state[4];
    uint64_t count; /* bits processed */
    unsigned char buffer[64];
    size_t buflen;
} md5_ctx;

static const uint32_t K[64] = {
    0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
    0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
    0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
    0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
    0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
    0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
    0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
    0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
};

static const int S[64] = {
    7,12,17,22, 7,12,17,22, 7,12,17,22, 7,12,17,22,
    5, 9,14,20, 5, 9,14,20, 5, 9,14,20, 5, 9,14,20,
    4,11,16,23, 4,11,16,23, 4,11,16,23, 4,11,16,23,
    6,10,15,21, 6,10,15,21, 6,10,15,21, 6,10,15,21
};

static uint32_t rotl(uint32_t x, int c) { return (x << c) | (x >> (32 - c)); }

static void md5_init(md5_ctx *ctx)
{
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
    ctx->count = 0;
    ctx->buflen = 0;
}

static void md5_process_block(md5_ctx *ctx, const unsigned char *block)
{
    uint32_t M[16];
    for (int i = 0; i < 16; i++) {
        M[i] = (uint32_t)block[i*4] | ((uint32_t)block[i*4+1] << 8) |
               ((uint32_t)block[i*4+2] << 16) | ((uint32_t)block[i*4+3] << 24);
    }

    uint32_t A = ctx->state[0], B = ctx->state[1], C = ctx->state[2], D = ctx->state[3];

    for (int i = 0; i < 64; i++) {
        uint32_t F;
        int g;
        if (i < 16) { F = (B & C) | (~B & D); g = i; }
        else if (i < 32) { F = (D & B) | (~D & C); g = (5*i + 1) % 16; }
        else if (i < 48) { F = B ^ C ^ D; g = (3*i + 5) % 16; }
        else { F = C ^ (B | ~D); g = (7*i) % 16; }

        F = F + A + K[i] + M[g];
        A = D; D = C; C = B;
        B = B + rotl(F, S[i]);
    }

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
}

static void md5_update(md5_ctx *ctx, const unsigned char *data, size_t len)
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
            md5_process_block(ctx, ctx->buffer);
            ctx->buflen = 0;
        }
    }
}

static void md5_final(md5_ctx *ctx, unsigned char digest[16])
{
    uint64_t bitcount = ctx->count;
    unsigned char pad = 0x80;
    md5_update(ctx, &pad, 1);

    unsigned char zero = 0;
    while (ctx->buflen != 56) {
        md5_update(ctx, &zero, 1);
    }

    unsigned char lenbytes[8];
    for (int i = 0; i < 8; i++) {
        lenbytes[i] = (unsigned char)(bitcount >> (8 * i));
    }
    memcpy(ctx->buffer + ctx->buflen, lenbytes, 8);
    md5_process_block(ctx, ctx->buffer);

    for (int i = 0; i < 4; i++) {
        digest[i*4]   = (unsigned char)(ctx->state[i]);
        digest[i*4+1] = (unsigned char)(ctx->state[i] >> 8);
        digest[i*4+2] = (unsigned char)(ctx->state[i] >> 16);
        digest[i*4+3] = (unsigned char)(ctx->state[i] >> 24);
    }
}

static int md5_file(FILE *f, unsigned char digest[16])
{
    md5_ctx ctx;
    md5_init(&ctx);

    unsigned char buf[8192];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        md5_update(&ctx, buf, n);
    }
    md5_final(&ctx, digest);
    return 0;
}

int md5sum_main(int argc, char **argv)
{
    unsigned char digest[16];

    if (argc < 2) {
        md5_file(stdin, digest);
        for (int i = 0; i < 16; i++) printf("%02x", digest[i]);
        printf("  -\n");
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "rb");
        if (!f) {
            rc |= die("md5sum", "cannot open", argv[i]);
            continue;
        }
        md5_file(f, digest);
        fclose(f);
        for (int j = 0; j < 16; j++) printf("%02x", digest[j]);
        printf("  %s\n", argv[i]);
    }

    return rc;
}
