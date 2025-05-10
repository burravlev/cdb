#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <stddef.h>
#include <sys/event.h>

typedef struct Server {
    int socket_listen_fd;
    int client_len;
    int socket_connection_fd;
    int kq;
    int new_events;
    struct kevent change_event[4];
    struct kevent event[4];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
} Server;

Server new_server(size_t port);
void server_start(Server* server, char* (*callback)(char*));

#endif