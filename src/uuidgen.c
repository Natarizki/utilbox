/* uuidgen.c — generate a random (version 4 style) UUID */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int uuidgen_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    srand((unsigned)(time(NULL) ^ getpid()));

    unsigned char b[16];
    for (int i = 0; i < 16; i++) b[i] = rand() & 0xFF;

    b[6] = (b[6] & 0x0F) | 0x40;
    b[8] = (b[8] & 0x3F) | 0x80;

    printf("%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
            b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7],
            b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15]);

    return 0;
}
