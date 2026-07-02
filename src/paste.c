/* paste.c — merge corresponding lines of multiple files, tab-separated */
#include <stdio.h>
#include <string.h>
#include "utilbox.h"

#define MAX_FILES 16

int paste_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("paste", "usage: paste FILE...");
    }

    int nfiles = argc - 1;
    if (nfiles > MAX_FILES) nfiles = MAX_FILES;

    FILE *files[MAX_FILES];
    for (int i = 0; i < nfiles; i++) {
        files[i] = fopen(argv[i + 1], "r");
        if (!files[i]) {
            die("paste", "cannot open", argv[i + 1]);
        }
    }

    char line[4096];
    int any_open;

    do {
        any_open = 0;
        for (int i = 0; i < nfiles; i++) {
            if (i > 0) putchar('\t');
            if (files[i] && fgets(line, sizeof(line), files[i]) != NULL) {
                size_t len = strlen(line);
                if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
                fputs(line, stdout);
                any_open = 1;
            }
        }
        if (any_open) putchar('\n');
    } while (any_open);

    for (int i = 0; i < nfiles; i++) {
        if (files[i]) fclose(files[i]);
    }

    return 0;
}
