/* groups.c — print the group memberships of the current user */
#include <stdio.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include "utilbox.h"

int groups_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    gid_t list[64];
    int n = getgroups(64, list);
    if (n < 0) {
        return die("groups", "cannot get groups", NULL);
    }

    for (int i = 0; i < n; i++) {
        struct group *gr = getgrgid(list[i]);
        if (i > 0) printf(" ");
        printf("%s", gr ? gr->gr_name : "?");
    }
    printf("\n");

    return 0;
}
