/* ifconfig.c — list network interfaces and their addresses via getifaddrs */
#include <stdio.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "utilbox.h"

int ifconfig_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) != 0) {
        return die("ifconfig", "getifaddrs failed", NULL);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;
        if (ifa->ifa_addr->sa_family != AF_INET) continue;

        struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
        char ipstr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr->sin_addr, ipstr, sizeof(ipstr));

        printf("%s: inet %s", ifa->ifa_name, ipstr);

        if (ifa->ifa_netmask) {
            struct sockaddr_in *mask = (struct sockaddr_in *)ifa->ifa_netmask;
            char maskstr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &mask->sin_addr, maskstr, sizeof(maskstr));
            printf(" netmask %s", maskstr);
        }
        printf(" flags=%s\n", (ifa->ifa_flags & 1) ? "UP" : "DOWN");
    }

    freeifaddrs(ifaddr);
    return 0;
}
