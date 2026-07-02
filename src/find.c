/* find.c — recursively search for files/directories.
 * Usage: find PATH -name PATTERN   (PATTERN uses simple glob: * and ?) */
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include "utilbox.h"

static void find_recursive(const char *path, const char *pattern)
{
    struct stat st;
    if (lstat(path, &st) != 0) return;

    const char *base = strrchr(path, '/');
    base = base ? base + 1 : path;

    if (pattern == NULL || fnmatch(pattern, base, 0) == 0) {
        puts(path);
    }

    if (S_ISDIR(st.st_mode)) {
        DIR *d = opendir(path);
        if (!d) return;

        struct dirent *ent;
        char child[4096];
        while ((ent = readdir(d)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
            snprintf(child, sizeof(child), "%s/%s", path, ent->d_name);
            find_recursive(child, pattern);
        }
        closedir(d);
    }
}

int find_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("find", "usage: find PATH [-name PATTERN]");
    }

    const char *path = argv[1];
    const char *pattern = NULL;

    if (argc >= 4 && strcmp(argv[2], "-name") == 0) {
        pattern = argv[3];
    }

    find_recursive(path, pattern);
    return 0;
}
