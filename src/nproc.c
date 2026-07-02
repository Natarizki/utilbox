/* nproc.c — print the number of available processing units */
#include <stdio.h>
#include <unistd.h>

int nproc_main(int argc, char **argv)
{
    (void)argc; (void)argv;
    long n = sysconf(_SC_NPROCESSORS_ONLN);
    printf("%ld\n", n > 0 ? n : 1);
    return 0;
}
