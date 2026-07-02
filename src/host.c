/* host.c — simple DNS lookup, prints IP addresses for a hostname */
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "utilbox.h"

int host_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("host", "usage: host HOSTNAME");
    }

    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(argv[1], NULL, &hints, &res);
    if (err != 0) {
        return fail("host", "cannot resolve hostname");
    }

    for (p = res; p != NULL; p = p->ai_next) {
        char ipstr[INET6_ADDRSTRLEN];
        void *addr;

        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &ipv4->sin_addr;
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &ipv6->sin6_addr;
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("%s has address %s\n", argv[1], ipstr);
    }

    freeaddrinfo(res);
    return 0;
}
