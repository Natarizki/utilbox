/* nc.c — minimal netcat. Usage: nc HOST PORT (connect) or nc -l PORT (listen).
 * Pipes stdin to socket and socket to stdout using a forked process. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "utilbox.h"

static void pipe_loop(int sock)
{
    pid_t pid = fork();
    if (pid == 0) {
        char buf[4096];
        ssize_t n;
        while ((n = read(0, buf, sizeof(buf))) > 0) {
            write(sock, buf, n);
        }
        _exit(0);
    }

    char buf[4096];
    ssize_t n;
    while ((n = read(sock, buf, sizeof(buf))) > 0) {
        write(1, buf, n);
    }
}

int nc_main(int argc, char **argv)
{
    if (argc >= 3 && strcmp(argv[1], "-l") == 0) {
        int port = atoi(argv[2]);

        int server = socket(AF_INET, SOCK_STREAM, 0);
        int yes = 1;
        setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(server, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
            return die("nc", "cannot bind", argv[2]);
        }
        listen(server, 1);

        int client = accept(server, NULL, NULL);
        close(server);
        if (client < 0) return die("nc", "accept failed", NULL);

        pipe_loop(client);
        close(client);
        return 0;
    }

    if (argc != 3) {
        return fail("nc", "usage: nc HOST PORT | nc -l PORT");
    }

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(argv[1], argv[2], &hints, &res) != 0) {
        return die("nc", "cannot resolve", argv[1]);
    }

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
        freeaddrinfo(res);
        return die("nc", "cannot connect to", argv[1]);
    }
    freeaddrinfo(res);

    pipe_loop(sock);
    close(sock);
    return 0;
}
