/* dmesg.c — print the kernel ring buffer (uses klogctl syscall) */
#include <stdio.h>
#include <sys/klog.h>
#include "utilbox.h"

int dmesg_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    char buf[16384];
    int n = klogctl(3, buf, sizeof(buf) - 1);
    if (n < 0) {
        return die("dmesg", "cannot read kernel log (need privilege)", NULL);
    }

    buf[n] = '\0';
    fputs(buf, stdout);
    return 0;
}
