/* chown.c — change file owner (and optionally group, via OWNER:GROUP). */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include "utilbox.h"

static int resolve_uid(const char *s)
{
    struct passwd *pw = getpwnam(s);
    if (pw) return pw->pw_uid;
    return atoi(s);
}

static int resolve_gid(const char *s)
{
    struct group *gr = getgrnam(s);
    if (gr) return gr->gr_gid;
    return atoi(s);
}

int chown_main(int argc, char **argv)
{
    if (argc < 3) {
        return fail("chown", "usage: chown OWNER[:GROUP] FILE...");
    }

    char spec[256];
    strncpy(spec, argv[1], sizeof(spec) - 1);
    spec[sizeof(spec) - 1] = '\0';

    uid_t uid;
    gid_t gid = (gid_t)-1;

    char *colon = strchr(spec, ':');
    if (colon) {
        *colon = '\0';
        gid = resolve_gid(colon + 1);
    }
    uid = resolve_uid(spec);

    int rc = 0;
    for (int i = 2; i < argc; i++) {
        if (chown(argv[i], uid, gid) != 0) {
            rc |= die("chown", "cannot change owner of", argv[i]);
        }
    }

    return rc;
}
