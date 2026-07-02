/* sync.c — flush filesystem buffers to disk */
#include <unistd.h>

int sync_main(int argc, char **argv)
{
    (void)argc; (void)argv;
    sync();
    return 0;
}
