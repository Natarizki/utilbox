#ifndef UTILBOX_H
#define UTILBOX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static inline int die(const char *applet, const char *msg, const char *arg)
{
    if (arg)
        fprintf(stderr, "utilbox: %s: %s '%s': %s\n", applet, msg, arg, strerror(errno));
    else
        fprintf(stderr, "utilbox: %s: %s: %s\n", applet, msg, strerror(errno));
    return 1;
}

static inline int fail(const char *applet, const char *msg)
{
    fprintf(stderr, "utilbox: %s: %s\n", applet, msg);
    return 1;
}

#endif
