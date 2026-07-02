/* more.c — simple pager: display a file one screenful at a time,
 * waiting for Enter between pages. */
#include <stdio.h>
#include "utilbox.h"

#define PAGE_SIZE 20

static void more_stream(FILE *in)
{
    char line[65536];
    int count = 0;

    while (fgets(line, sizeof(line), in) != NULL) {
        fputs(line, stdout);
        count++;
        if (count % PAGE_SIZE == 0) {
            printf("--More--");
            fflush(stdout);
            int c = getchar();
            if (c == 'q') break;
            printf("\r        \r");
        }
    }
}

int more_main(int argc, char **argv)
{
    if (argc < 2) {
        more_stream(stdin);
        return 0;
    }

    int rc = 0;
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("more", "cannot open", argv[i]);
            continue;
        }
        more_stream(f);
        fclose(f);
    }

    return rc;
}
