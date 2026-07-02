/* tar.c — minimal ustar-format archiver. Supports -c (create), -x (extract),
 * -t (list), -f FILE. No compression, no directory recursion. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "utilbox.h"

struct tar_header {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char pad[12];
};

static unsigned compute_checksum(struct tar_header *h)
{
    memset(h->chksum, ' ', 8);
    unsigned sum = 0;
    unsigned char *p = (unsigned char *)h;
    for (int i = 0; i < 512; i++) sum += p[i];
    return sum;
}

static void write_octal(char *field, size_t fieldlen, unsigned long value)
{
    snprintf(field, fieldlen, "%0*lo", (int)fieldlen - 1, value);
}

static int tar_create(const char *archive, char **files, int nfiles)
{
    FILE *out = fopen(archive, "wb");
    if (!out) return die("tar", "cannot create", archive);

    for (int i = 0; i < nfiles; i++) {
        struct stat st;
        if (stat(files[i], &st) != 0) {
            die("tar", "cannot stat", files[i]);
            continue;
        }

        struct tar_header h;
        memset(&h, 0, sizeof(h));
        strncpy(h.name, files[i], sizeof(h.name) - 1);
        write_octal(h.mode, sizeof(h.mode), st.st_mode & 0777);
        write_octal(h.uid, sizeof(h.uid), st.st_uid);
        write_octal(h.gid, sizeof(h.gid), st.st_gid);
        write_octal(h.size, sizeof(h.size), st.st_size);
        write_octal(h.mtime, sizeof(h.mtime), st.st_mtime);
        h.typeflag = '0';
        memcpy(h.magic, "ustar", 5);
        h.magic[5] = '\0';
        memcpy(h.version, "00", 2);

        unsigned sum = compute_checksum(&h);
        snprintf(h.chksum, sizeof(h.chksum), "%06o", sum);
        h.chksum[6] = '\0';
        h.chksum[7] = ' ';

        fwrite(&h, 1, 512, out);

        FILE *in = fopen(files[i], "rb");
        if (in) {
            char buf[512];
            size_t n;
            while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
                if (n < 512) memset(buf + n, 0, 512 - n);
                fwrite(buf, 1, 512, out);
            }
            fclose(in);
        }
    }

    char zeroblock[1024];
    memset(zeroblock, 0, sizeof(zeroblock));
    fwrite(zeroblock, 1, sizeof(zeroblock), out);

    fclose(out);
    return 0;
}

static int tar_extract_or_list(const char *archive, int list_only)
{
    FILE *in = fopen(archive, "rb");
    if (!in) return die("tar", "cannot open", archive);

    struct tar_header h;
    while (fread(&h, 1, 512, in) == 512) {
        if (h.name[0] == '\0') break;

        long size = strtol(h.size, NULL, 8);

        if (list_only) {
            printf("%s\n", h.name);
        } else {
            FILE *out = fopen(h.name, "wb");
            if (!out) {
                die("tar", "cannot create", h.name);
            }
            long remaining = size;
            char buf[512];
            while (remaining > 0) {
                fread(buf, 1, 512, in);
                size_t towrite = (remaining < 512) ? (size_t)remaining : 512;
                if (out) fwrite(buf, 1, towrite, out);
                remaining -= 512;
            }
            if (out) fclose(out);
            continue;
        }

        long blocks = (size + 511) / 512;
        fseek(in, blocks * 512, SEEK_CUR);
    }

    fclose(in);
    return 0;
}

int tar_main(int argc, char **argv)
{
    if (argc < 3) {
        return fail("tar", "usage: tar -c|-x|-t -f ARCHIVE [FILES...]");
    }

    const char *mode = argv[1];
    const char *archive = NULL;
    int fi = 2;

    if (strcmp(argv[2], "-f") == 0 && argc >= 4) {
        archive = argv[3];
        fi = 4;
    } else {
        return fail("tar", "usage: tar -c|-x|-t -f ARCHIVE [FILES...]");
    }

    if (strchr(mode, 'c')) {
        return tar_create(archive, argv + fi, argc - fi);
    }
    else if (strchr(mode, 'x')) {
        return tar_extract_or_list(archive, 0);
    }
    else if (strchr(mode, 't')) {
        return tar_extract_or_list(archive, 1);
    }

    return fail("tar", "unknown mode (use -c, -x, or -t)");
}
