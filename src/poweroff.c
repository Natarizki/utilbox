/* poweroff.c — power off the system (requires PID 1 / CAP_SYS_BOOT) */
#include <unistd.h>
#include <sys/reboot.h>
#include <linux/reboot.h>
#include "utilbox.h"

int poweroff_main(int argc, char **argv)
{
    (void)argc; (void)argv;
    sync();
    if (reboot(LINUX_REBOOT_CMD_POWER_OFF) != 0) {
        return die("poweroff", "cannot power off (need privilege / pid 1)", NULL);
    }
    return 0;
}
