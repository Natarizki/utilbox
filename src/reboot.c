/* reboot.c — restart the system (requires PID 1 / CAP_SYS_BOOT) */
#include <unistd.h>
#include <sys/reboot.h>
#include <linux/reboot.h>
#include "utilbox.h"

int reboot_main(int argc, char **argv)
{
    (void)argc; (void)argv;
    sync();
    if (reboot(LINUX_REBOOT_CMD_RESTART) != 0) {
        return die("reboot", "cannot reboot (need privilege / pid 1)", NULL);
    }
    return 0;
}
