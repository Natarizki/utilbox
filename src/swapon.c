/* swapon.c — enable a swap file or partition */
#include <sys/swap.h>
#include "utilbox.h"

int swapon_main(int argc, char **argv)
{
    if (argc != 2) {
        return fail("swapon", "usage: swapon DEVICE");
    }

    if (swapon(argv[1], 0) != 0) {
        return die("swapon", "cannot enable swap", argv[1]);
    }

    return 0;
}
