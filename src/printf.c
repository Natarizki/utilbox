/* printf.c — simplified printf command. Supports %s, %d, %c, %%, and
 * escape sequences \n \t \\ in the format string. Extra args beyond
 * format specifiers are ignored (v1 limitation: no format re-use loop). */
#include <stdio.h>
#include <stdlib.h>
#include "utilbox.h"

int printf_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("printf", "usage: printf FORMAT [ARGS...]");
    }

    const char *fmt = argv[1];
    int argi = 2;

    for (const char *p = fmt; *p; p++) {
        if (*p == '\\' && *(p + 1)) {
            p++;
            switch (*p) {
                case 'n': putchar('\n'); break;
                case 't': putchar('\t'); break;
                case '\\': putchar('\\'); break;
                default: putchar(*p); break;
            }
        }
        else if (*p == '%' && *(p + 1)) {
            p++;
            switch (*p) {
                case 's':
                    if (argi < argc) fputs(argv[argi++], stdout);
                    break;
                case 'd':
                    if (argi < argc) printf("%ld", atol(argv[argi++]));
                    break;
                case 'c':
                    if (argi < argc) putchar(argv[argi++][0]);
                    break;
                case '%':
                    putchar('%');
                    break;
                default:
                    putchar('%');
                    putchar(*p);
                    break;
            }
        }
        else {
            putchar(*p);
        }
    }

    return 0;
}
