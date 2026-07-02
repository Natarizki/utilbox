/* ping.c — send ICMP echo requests (requires raw socket privilege).
 * Simplified: fixed count of 4 pings, no fancy statistics formatting. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include "utilbox.h"

static unsigned short in_checksum(void *data, int len)
{
    unsigned short *buf = data;
    unsigned int sum = 0;
    for (; len > 1; len -= 2) sum += *buf++;
    if (len == 1) sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

int ping_main(int argc, char **argv)
{
    if (argc < 2) {
        return fail("ping", "usage: ping HOST");
    }

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;

    if (getaddrinfo(argv[1], NULL, &hints, &res) != 0) {
        return die("ping", "cannot resolve host", argv[1]);
    }

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        freeaddrinfo(res);
        return die("ping", "cannot create raw socket (need privilege)", NULL);
    }

    struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr->sin_addr, ipstr, sizeof(ipstr));
    printf("PING %s (%s)\n", argv[1], ipstr);

    for (int seq = 0; seq < 4; seq++) {
        struct {
            struct icmphdr hdr;
            char msg[32];
        } packet;
        memset(&packet, 0, sizeof(packet));
        packet.hdr.type = ICMP_ECHO;
        packet.hdr.un.echo.id = getpid() & 0xFFFF;
        packet.hdr.un.echo.sequence = seq;
        strcpy(packet.msg, "utilbox ping");
        packet.hdr.checksum = in_checksum(&packet, sizeof(packet));

        sendto(sock, &packet, sizeof(packet), 0, res->ai_addr, res->ai_addrlen);

        char buf[512];
        struct sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        ssize_t n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen);
        if (n > 0) {
            printf("reply from %s: seq=%d\n", ipstr, seq);
        }
        sleep(1);
    }

    close(sock);
    freeaddrinfo(res);
    return 0;
}
