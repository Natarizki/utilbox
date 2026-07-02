#include <stdio.h>
#include <string.h>
#include "applets.h"

static const char *basename_of(const char *path)
{
    const char *slash = strrchr(path, '/');
    return slash ? slash + 1 : path;
}

static void print_usage(void)
{
    fprintf(stderr, "utilbox: a multi-call binary. Usage:\n");
    fprintf(stderr, "  utilbox <applet> [args...]\n");
    fprintf(stderr, "  or symlink/hardlink utilbox to an applet name (e.g. ln -s utilbox ls)\n\n");
    fprintf(stderr, "Built-in applets:\n");
    for (int i = 0; applets[i].name != NULL; i++) {
        fprintf(stderr, "  %s\n", applets[i].name);
    }
}

static applet_fn find_applet(const char *name)
{
    for (int i = 0; applets[i].name != NULL; i++) {
        if (strcmp(applets[i].name, name) == 0) {
            return applets[i].main;
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 1) {
        print_usage();
        return 1;
    }

    const char *invoked_name = basename_of(argv[0]);

    applet_fn fn = find_applet(invoked_name);
    if (fn != NULL) {
        return fn(argc, argv);
    }

    if (argc >= 2) {
        fn = find_applet(argv[1]);
        if (fn != NULL) {
            return fn(argc - 1, argv + 1);
        }
        fprintf(stderr, "utilbox: applet '%s' not found\n", argv[1]);
        return 1;
    }

    print_usage();
    return 1;
}
