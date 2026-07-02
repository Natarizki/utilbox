/* ls.c — list directory contents. Supports -a (show hidden) and -l (long format). */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "utilbox.h"

static void mode_to_str(mode_t m, char *out)
{
    out[0] = S_ISDIR(m) ? 'd' : S_ISLNK(m) ? 'l' : '-';
    out[1] = (m & S_IRUSR) ? 'r' : '-';
    out[2] = (m & S_IWUSR) ? 'w' : '-';
    out[3] = (m & S_IXUSR) ? 'x' : '-';
    out[4] = (m & S_IRGRP) ? 'r' : '-';
    out[5] = (m & S_IWGRP) ? 'w' : '-';
    out[6] = (m & S_IXGRP) ? 'x' : '-';
    out[7] = (m & S_IROTH) ? 'r' : '-';
    out[8] = (m & S_IWOTH) ? 'w' : '-';
    out[9] = (m & S_IXOTH) ? 'x' : '-';
    out[10] = '\0';
}

static int cmp_names(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

static int list_dir(const char *path, int show_all, int long_fmt)
{
    DIR *d = opendir(path);
    if (!d) return die("ls", "cannot open directory", path);

    char **names = NULL;
    size_t count = 0, cap = 0;
    struct dirent *ent;

    while ((ent = readdir(d)) != NULL) {
        if (!show_all && ent->d_name[0] == '.') continue;
        if (count == cap) {
            cap = cap ? cap * 2 : 32;
            names = realloc(names, cap * sizeof(char *));
        }
        names[count++] = strdup(ent->d_name);
    }
    closedir(d);

    qsort(names, count, sizeof(char *), cmp_names);

    for (size_t i = 0; i < count; i++) {
        if (long_fmt) {
            char full[4096];
            snprintf(full, sizeof(full), "%s/%s", path, names[i]);
            struct stat st;
            if (lstat(full, &st) != 0) {
                fprintf(stderr, "utilbox: ls: cannot stat '%s'\n", full);
                continue;
            }
            char modestr[11];
            mode_to_str(st.st_mode, modestr);

            struct passwd *pw = getpwuid(st.st_uid);
            struct group *gr = getgrgid(st.st_gid);

            char timebuf[32];
            struct tm *tm_info = localtime(&st.st_mtime);
            strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);

            printf("%s %3lu %-8s %-8s %8ld %s %s\n",
                    modestr,
                    (unsigned long)st.st_nlink,
                    pw ? pw->pw_name : "?",
                    gr ? gr->gr_name : "?",
                    (long)st.st_size,
                    timebuf,
                    names[i]);
        }
        else {
            printf("%s\n", names[i]);
        }
        free(names[i]);
    }
    free(names);

    return 0;
}

int ls_main(int argc, char **argv)
{
    int show_all = 0, long_fmt = 0;
    int i = 1;

    for (; i < argc; i++) {
        if (argv[i][0] != '-') break;
        if (strchr(argv[i], 'a')) show_all = 1;
        if (strchr(argv[i], 'l')) long_fmt = 1;
    }

    if (i >= argc) {
        return list_dir(".", show_all, long_fmt);
    }

    int rc = 0;
    for (; i < argc; i++) {
        if (argc - i > 1) printf("%s:\n", argv[i]);
        rc |= list_dir(argv[i], show_all, long_fmt);
    }

    return rc;
}
