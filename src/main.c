#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <string.h>
#include <unistd.h>
#include <db.h>
#include <hashtable.h>

int main() {
    h_table *ht = ht_new();
    int socket_listen_fd,
        portno = 1815,
        client_len,
        socket_connection_fd,
        kq,
        new_events;
    struct kevent change_event[4],
        event[4];
    struct sockaddr_in serv_addr,
        client_addr;

    if (((socket_listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)) {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(socket_listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    listen(socket_listen_fd, 3);
    client_len = sizeof(client_addr);

    kq = kqueue();

    EV_SET(change_event, socket_listen_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

    if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1) {
        perror("kevent");
        exit(1);
    }

    for (;;) {
        new_events = kevent(kq, NULL, 0, event, 1, NULL);
        if (new_events == -1) {
            perror("kevent");
            exit(1);
        }

        for (int i = 0; new_events > i; i++) {
            int event_fd = event[i].ident;

            if (event[i].flags & EV_EOF) {
                printf("Client has disconnected");
                close(event_fd);
            }
            else if (event_fd == socket_listen_fd) {
                socket_connection_fd = accept(event_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
                if (socket_connection_fd == -1) {
                    perror("Accept socket error");
                }

                EV_SET(change_event, socket_connection_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(kq, change_event, 1, NULL, 0, NULL) < 0) {
                    perror("kevent error");
                }
            }

            else if (event[i].filter & EVFILT_READ) {
                char buf[1024];
                size_t bytes_read = recv(event_fd, buf, sizeof(buf), 0);
                char *result = read_cmd(ht, buf);
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
    ht_free(ht);
    return 0;
}
