/* id.c — print user and group IDs */
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include "utilbox.h"

int id_main(int argc, char **argv)
{
    (void)argc;
    uid_t uid = geteuid();
    gid_t gid = getegid();

    struct passwd *pw = getpwuid(uid);
    struct group *gr = getgrgid(gid);

    printf("uid=%d(%s) gid=%d(%s)\n",
            (int)uid, pw ? pw->pw_name : "?",
            (int)gid, gr ? gr->gr_name : "?");

    (void)argv;
    return 0;
}
