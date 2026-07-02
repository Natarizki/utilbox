/* file.c — simplified file type detection based on stat + magic byte sniffing. */
#include <stdio.h>
#include <sys/stat.h>
#include "utilbox.h"

static void describe(const char *path)
{
    struct stat st;
    if (lstat(path, &st) != 0) {
        printf("%s: cannot open (%s)\n", path, "no such file or directory");
        return;
    }

    if (S_ISDIR(st.st_mode)) { printf("%s: directory\n", path); return; }
    if (S_ISLNK(st.st_mode)) { printf("%s: symbolic link\n", path); return; }
    if (S_ISFIFO(st.st_mode)) { printf("%s: fifo (named pipe)\n", path); return; }
    if (S_ISSOCK(st.st_mode)) { printf("%s: socket\n", path); return; }
    if (S_ISCHR(st.st_mode)) { printf("%s: character special file\n", path); return; }
    if (S_ISBLK(st.st_mode)) { printf("%s: block special file\n", path); return; }

    FILE *f = fopen(path, "rb");
    if (!f) { printf("%s: regular file (unreadable)\n", path); return; }

    unsigned char magic[4] = {0};
    size_t n = fread(magic, 1, 4, f);
    fclose(f);

    if (n >= 4 && magic[0] == 0x7f && magic[1] == 'E' && magic[2] == 'L' && magic[3] == 'F') {
        printf("%s: ELF binary\n", path);
    }
    else if (n >= 2 && magic[0] == '#' && magic[1] == '!') {
        printf("%s: script text executable\n", path);
    }
    else if (n >= 4 && magic[0] == 0x89 && magic[1] == 'P' && magic[2] == 'N' && magic[3] == 'G') {
        printf("%s: PNG image\n", path);
    }
    else if (st.st_size == 0) {
        printf("%s: empty\n", path);
    }
    else {
        printf("%s: data\n", path);
    }
}

int file_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("file", "usage: file FILE...");
    }
    for (int i = 1; i < argc; i++) {
        describe(argv[i]);
    }
    return 0;
}
