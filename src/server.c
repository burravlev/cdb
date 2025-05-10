#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_socklen_t.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>

#include "../lib/server.h"

Server new_server(size_t port) {
    Server server;
    if ((server.socket_listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ERROR opening socket!");
        exit(1);
    }
    bzero((char *)&server.server_addr, sizeof(server.server_addr));
    server.server_addr.sin_family = AF_INET;
    server.server_addr.sin_addr.s_addr = INADDR_ANY;
    server.server_addr.sin_port = htons(port);
    return server;
}

void server_start(Server* server, char*(*callback)(char*)) {
    if (bind(server->socket_listen_fd, (struct sockaddr *)&(server->server_addr), sizeof(server->server_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    listen(server->socket_listen_fd, 3);
    server->client_len = sizeof(server->client_addr);

    server->kq = kqueue();

    EV_SET(server->change_event, server->socket_listen_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

    if (kevent(server->kq, server->change_event, 1, NULL, 0, NULL) == -1) {
        perror("kevent");
        exit(1);
    }

    for (;;) {
        server->new_events = kevent(server->kq, NULL, 0, server->event, 1, NULL);
        if (server->new_events == -1) {
            perror("kevent");
            exit(1);
        }

        for (int i = 0; server->new_events > i; i++) {
            int event_fd = server->event[i].ident;

            if (server->event[i].flags & EV_EOF) {
                printf("Client has disconnected");
                close(event_fd);
            }
            else if (event_fd == server->socket_listen_fd) {
                server->socket_connection_fd = accept(event_fd, (struct sockaddr *)&(server->client_addr), (socklen_t *)&(server->client_len));
                if (server->socket_connection_fd == -1) {
                    perror("Accept socket error");
                }

                EV_SET(server->change_event, server->socket_connection_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(server->kq, server->change_event, 1, NULL, 0, NULL) < 0) {
                    perror("kevent error");
                }
            }

            else if (server->event[i].filter & EVFILT_READ) {
                char buf[1024];
                size_t bytes_read = recv(event_fd, buf, sizeof(buf), 0);
                char *result = callback(buf);
                printf("result: %s\n", result);
                bzero(buf, 1024);
                if (result == NULL) {
                    send(event_fd, NULL, 0, 0);
                } else {
                    send(event_fd, result, strlen(result), 0);
                }
            }
        }
    }
}