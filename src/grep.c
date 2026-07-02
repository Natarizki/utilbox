/* grep.c — search for lines matching a pattern (plain substring match, not
 * full regex, in v1). Supports -i (ignore case), -v (invert match),
 * -n (show line numbers). */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utilbox.h"

static char *str_to_lower(const char *s, char *out, size_t outsz)
{
    size_t i;
    for (i = 0; s[i] && i < outsz - 1; i++) {
        out[i] = tolower((unsigned char)s[i]);
    }
    out[i] = '\0';
    return out;
}

static int grep_stream(FILE *in, const char *pattern, int ignore_case, int invert,
        int show_lineno, const char *fname, int show_fname)
{
    char line[65536];
    long lineno = 0;
    int found_any = 0;

    char pat_lower[512];
    if (ignore_case) str_to_lower(pattern, pat_lower, sizeof(pat_lower));

    while (fgets(line, sizeof(line), in) != NULL) {
        lineno++;
        int matched;

        if (ignore_case) {
            char line_lower[65536];
            str_to_lower(line, line_lower, sizeof(line_lower));
            matched = strstr(line_lower, pat_lower) != NULL;
        } else {
            matched = strstr(line, pattern) != NULL;
        }

        if (invert) matched = !matched;

        if (matched) {
            found_any = 1;
            if (show_fname) printf("%s:", fname);
            if (show_lineno) printf("%ld:", lineno);
            fputs(line, stdout);
            if (line[strlen(line) - 1] != '\n') printf("\n");
        }
    }

    return found_any;
}

int grep_main(int argc, char **argv)
{
    int ignore_case = 0, invert = 0, show_lineno = 0;
    int i = 1;

    for (; i < argc && argv[i][0] == '-' && argv[i][1] != '\0'; i++) {
        if (strchr(argv[i], 'i')) ignore_case = 1;
        if (strchr(argv[i], 'v')) invert = 1;
        if (strchr(argv[i], 'n')) show_lineno = 1;
    }

    if (i >= argc) {
        return fail("grep", "usage: grep [-inv] PATTERN [FILE...]");
    }

    const char *pattern = argv[i++];
    int any_match = 0;

    if (i >= argc) {
        any_match = grep_stream(stdin, pattern, ignore_case, invert, show_lineno, "-", 0);
        return any_match ? 0 : 1;
    }

    int show_fname = (argc - i) > 1;
    for (; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            die("grep", "cannot open", argv[i]);
            continue;
        }
        if (grep_stream(f, pattern, ignore_case, invert, show_lineno, argv[i], show_fname)) {
            any_match = 1;
        }
        fclose(f);
    }

    return any_match ? 0 : 1;
}
