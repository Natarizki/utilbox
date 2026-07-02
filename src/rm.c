/* rm.c — remove files and directories. Supports -r (recursive) and -f (force). */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "utilbox.h"

static int rm_recursive(const char *path, int force)
{
    struct stat st;
    if (lstat(path, &st) != 0) {
        if (force) return 0;
        return die("rm", "cannot stat", path);
    }

    if (S_ISDIR(st.st_mode)) {
        DIR *d = opendir(path);
        if (!d) {
            if (force) return 0;
            return die("rm", "cannot open directory", path);
        }

        struct dirent *ent;
        int rc = 0;
        char child[4096];
        while ((ent = readdir(d)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
            snprintf(child, sizeof(child), "%s/%s", path, ent->d_name);
            rc |= rm_recursive(child, force);
        }
        closedir(d);

        if (rmdir(path) != 0 && !force) {
            rc |= die("rm", "cannot remove directory", path);
        }
        return rc;
    }
    else {
        if (unlink(path) != 0 && !force) {
            return die("rm", "cannot remove", path);
        }
        return 0;
    }
}

int rm_main(int argc, char **argv)
{
    int recursive = 0, force = 0;
    int i = 1;

    for (; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "-rf") == 0 || strcmp(argv[i], "-fr") == 0) {
            recursive = 1;
            if (strlen(argv[i]) > 2) force = 1;
        }
        else if (strcmp(argv[i], "-f") == 0) {
            force = 1;
        }
        else {
            break;
        }
    }

    if (i >= argc) {
        return fail("rm", "missing operand");
    }

    int rc = 0;
    for (; i < argc; i++) {
        if (recursive) {
            rc |= rm_recursive(argv[i], force);
        }
        else if (unlink(argv[i]) != 0 && !force) {
            rc |= die("rm", "cannot remove", argv[i]);
        }
    }

    return rc;
}
