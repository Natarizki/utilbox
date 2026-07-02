/* banner.c — print text in large block-style letters using repeated
 * character blocks (simplified: not true per-letter bitmap fonts). */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utilbox.h"

int banner_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("banner", "usage: banner TEXT");
    }

    const char *text = argv[1];
    size_t len = strlen(text);

    for (int row = 0; row < 5; row++) {
        for (size_t i = 0; i < len; i++) {
            char c = toupper((unsigned char)text[i]);
            if (c == ' ') {
                printf("      ");
                continue;
            }
            if (row == 0 || row == 4) printf("###### ");
            else printf("#    # ");
        }
        printf("\n");
    }

    return 0;
}
