/* cp.c — copy files and directories. Supports -r (recursive). */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include "utilbox.h"

static int copy_file(const char *src, const char *dst)
{
    int in = open(src, O_RDONLY);
    if (in < 0) return die("cp", "cannot open", src);

    struct stat st;
    fstat(in, &st);

    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, st.st_mode & 0777);
    if (out < 0) {
        close(in);
        return die("cp", "cannot create", dst);
    }

    char buf[8192];
    ssize_t n;
    int rc = 0;
    while ((n = read(in, buf, sizeof(buf))) > 0) {
        if (write(out, buf, n) != n) {
            rc = die("cp", "write error", dst);
            break;
        }
    }
    if (n < 0) rc = die("cp", "read error", src);

    close(in);
    close(out);
    return rc;
}

static int copy_recursive(const char *src, const char *dst)
{
    struct stat st;
    if (lstat(src, &st) != 0) {
        return die("cp", "cannot stat", src);
    }

    if (S_ISDIR(st.st_mode)) {
        if (mkdir(dst, st.st_mode & 0777) != 0 && errno != EEXIST) {
            return die("cp", "cannot create directory", dst);
        }

        DIR *d = opendir(src);
        if (!d) return die("cp", "cannot open directory", src);

        struct dirent *ent;
        int rc = 0;
        char child_src[4096], child_dst[4096];
        while ((ent = readdir(d)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
            snprintf(child_src, sizeof(child_src), "%s/%s", src, ent->d_name);
            snprintf(child_dst, sizeof(child_dst), "%s/%s", dst, ent->d_name);
            rc |= copy_recursive(child_src, child_dst);
        }
        closedir(d);
        return rc;
    }
    else {
        return copy_file(src, dst);
    }
}

int cp_main(int argc, char **argv)
{
    int recursive = 0;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-r") == 0) {
        recursive = 1;
        i++;
    }

    if (i + 2 > argc) {
        return fail("cp", "usage: cp [-r] SOURCE... DEST");
    }

    const char *dst = argv[argc - 1];
    int rc = 0;

    for (; i < argc - 1; i++) {
        if (recursive) {
            rc |= copy_recursive(argv[i], dst);
        }
        else {
            rc |= copy_file(argv[i], dst);
        }
    }

    return rc;
}
