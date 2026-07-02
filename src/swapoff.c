/* swapoff.c — disable a swap file or partition */
#include <sys/swap.h>
#include "utilbox.h"

int swapoff_main(int argc, char **argv)
{
    if (argc != 2) {
        return fail("swapoff", "usage: swapoff DEVICE");
    }

    if (swapoff(argv[1]) != 0) {
        return die("swapoff", "cannot disable swap", argv[1]);
    }

    return 0;
}
