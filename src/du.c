/* du.c — estimate disk usage of files/directories, recursively.
 * Prints size in KB (like `du -k`) for each argument. */
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "utilbox.h"

static long du_recursive(const char *path)
{
    struct stat st;
    if (lstat(path, &st) != 0) return 0;

    long total = st.st_blocks * 512 / 1024; /* KB */

    if (S_ISDIR(st.st_mode)) {
        DIR *d = opendir(path);
        if (!d) return total;

        struct dirent *ent;
        char child[4096];
        while ((ent = readdir(d)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
            snprintf(child, sizeof(child), "%s/%s", path, ent->d_name);
            total += du_recursive(child);
        }
        closedir(d);
    }

    return total;
}

int du_main(int argc, char **argv)
{
    if (argc < 2) {
        printf("%ld\t.\n", du_recursive("."));
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        printf("%ld\t%s\n", du_recursive(argv[i]), argv[i]);
    }

    return 0;
}
