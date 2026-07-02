/* netstat.c — display active TCP connections, parsed from /proc/net/tcp */
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "utilbox.h"

static const char *tcp_state_name(int state)
{
    static const char *names[] = {
        "UNKNOWN", "ESTABLISHED", "SYN_SENT", "SYN_RECV", "FIN_WAIT1",
        "FIN_WAIT2", "TIME_WAIT", "CLOSE", "CLOSE_WAIT", "LAST_ACK",
        "LISTEN", "CLOSING"
    };
    if (state < 0 || state > 11) return "UNKNOWN";
    return names[state];
}

int netstat_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    FILE *f = fopen("/proc/net/tcp", "r");
    if (!f) return die("netstat", "cannot open /proc/net/tcp", NULL);

    char line[512];
    fgets(line, sizeof(line), f);

    printf("%-22s %-22s %s\n", "Local Address", "Remote Address", "State");

    unsigned laddr, raddr;
    unsigned lport, rport;
    int state;

    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "%*d: %x:%x %x:%x %x", &laddr, &lport, &raddr, &rport, &state) == 5) {
            struct in_addr la = { .s_addr = laddr };
            struct in_addr ra = { .s_addr = raddr };
            char lbuf[64], rbuf[64];
            snprintf(lbuf, sizeof(lbuf), "%s:%u", inet_ntoa(la), lport);
            snprintf(rbuf, sizeof(rbuf), "%s:%u", inet_ntoa(ra), rport);
            printf("%-22s %-22s %s\n", lbuf, rbuf, tcp_state_name(state));
        }
    }

    fclose(f);
    return 0;
}
