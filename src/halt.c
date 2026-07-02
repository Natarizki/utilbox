/* halt.c — halt the system (requires PID 1 / CAP_SYS_BOOT) */
#include <unistd.h>
#include <sys/reboot.h>
#include <linux/reboot.h>
#include "utilbox.h"

int halt_main(int argc, char **argv)
{
    (void)argc; (void)argv;
    sync();
    if (reboot(LINUX_REBOOT_CMD_HALT) != 0) {
        return die("halt", "cannot halt (need privilege / pid 1)", NULL);
    }
    return 0;
}
