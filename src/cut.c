/* cut.c — extract fields from each line. Supports -d DELIM (default tab)
 * and -f FIELDS (comma-separated list of 1-based field numbers). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilbox.h"

#define MAX_FIELDS 256

static int parse_fields(const char *spec, int *fields)
{
    int n = 0;
    char buf[1024];
    strncpy(buf, spec, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    char *tok = strtok(buf, ",");
    while (tok != NULL && n < MAX_FIELDS) {
        fields[n++] = atoi(tok);
        tok = strtok(NULL, ",");
    }
    return n;
}

static int field_in_list(int f, const int *fields, int n)
{
    for (int i = 0; i < n; i++) {
        if (fields[i] == f) return 1;
    }
    return 0;
}

static void cut_line(char *line, char delim, const int *fields, int nfields)
{
    char *parts[MAX_FIELDS];
    int nparts = 0;

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

    char *p = line;
    parts[nparts++] = p;
    for (; *p && nparts < MAX_FIELDS; p++) {
        if (*p == delim) {
            *p = '\0';
            parts[nparts++] = p + 1;
        }
    }

    int first = 1;
    for (int i = 1; i <= nparts; i++) {
        if (field_in_list(i, fields, nfields)) {
            if (!first) putchar(delim);
            fputs(parts[i - 1], stdout);
            first = 0;
        }
    }
    putchar('\n');
}

int cut_main(int argc, char **argv)
{
    char delim = '\t';
    int fields[MAX_FIELDS];
    int nfields = 0;
    int i = 1;

    for (; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            delim = argv[++i][0];
        }
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            nfields = parse_fields(argv[++i], fields);
        }
        else {
            break;
        }
    }

    if (nfields == 0) {
        return fail("cut", "usage: cut -d DELIM -f FIELDS [FILE...]");
    }

    char line[65536];

    if (i >= argc) {
        while (fgets(line, sizeof(line), stdin) != NULL) {
            cut_line(line, delim, fields, nfields);
        }
        return 0;
    }

    int rc = 0;
    for (; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            rc |= die("cut", "cannot open", argv[i]);
            continue;
        }
        while (fgets(line, sizeof(line), f) != NULL) {
            cut_line(line, delim, fields, nfields);
        }
        fclose(f);
    }

    return rc;
}
