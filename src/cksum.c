/* cksum.c — compute CRC32 checksum and byte count of files (standard
 * CRC32 polynomial table, same algorithm used by zip/gzip). */
#include <stdio.h>
#include "utilbox.h"

static unsigned long crc_table[256];
static int table_built = 0;

static void build_table(void)
{
    for (unsigned long i = 0; i < 256; i++) {
        unsigned long c = i;
        for (int j = 0; j < 8; j++) {
            c = (c & 1) ? (0xEDB88320UL ^ (c >> 1)) : (c >> 1);
        }
        crc_table[i] = c;
    }
    table_built = 1;
}

static int cksum_stream(FILE *in, unsigned long *crc_out, unsigned long *len_out)
{
    if (!table_built) build_table();

    unsigned long crc = 0xFFFFFFFFUL;
    unsigned long len = 0;
    unsigned char buf[8192];
    size_t n;

    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        for (size_t i = 0; i < n; i++) {
            crc = crc_table[(crc ^ buf[i]) & 0xFF] ^ (crc >> 8);
        }
        len += n;
    }

    *crc_out = crc ^ 0xFFFFFFFFUL;
    *len_out = len;
    return 0;
}

int cksum_main(int argc, char **argv)
{
    if (argc < 2) {
        unsigned long crc, len;
        cksum_stream(stdin, &crc, &len);
        printf("%lu %lu\n", crc, len);
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "rb");
        if (!f) {
            rc |= die("cksum", "cannot open", argv[i]);
            continue;
        }
        unsigned long crc, len;
        cksum_stream(f, &crc, &len);
        fclose(f);
        printf("%lu %lu %s\n", crc, len, argv[i]);
    }

    return rc;
}
