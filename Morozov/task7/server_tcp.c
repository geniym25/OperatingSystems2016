//
// Created by vks on 22.05.16.
//

#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <zconf.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    const size_t max_len = 1024;
    const uint16_t port = 4242;
    int sock, client_sock;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t client_len;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        fprintf(stderr, "Error creating socket.\n");
        return 1;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) {
        fprintf(stderr, "Error binding socket.\n");
        return 1;
    }
    if (listen(sock, 5) == -1) {
        fprintf(stderr, "Error while listening.\n");
        return 1;
    }

    client_len = sizeof(struct sockaddr_in);
    ssize_t count;
    char *msg = (char *) malloc(max_len * sizeof(char));
    char *ans = (char *) malloc(max_len * sizeof(char));

    while (1) {
        client_sock = accept(sock, (struct sockaddr *) &cli_addr, &client_len);
        pid_t pr0 = fork();
        if (pr0 < 0) {
            fprintf(stderr, "Error forking.\n");
            return 1;
        }
        if (pr0 == 0) {
            while (1) {
                bzero(msg, max_len * sizeof(char));
                bzero(ans, max_len * sizeof(char));
                if (client_sock < 0) {
                    fprintf(stderr, "Error accepting connection.\n");
                    return 1;
                }
                count = recv(client_sock, msg, max_len * sizeof(char), 0);
                if (count < 0) {
                    fprintf(stderr, "Error reading message.\n");
                }
                if (strcmp(msg, "srv_exit") == 0) {
                    printf("*** Received shutdown message.\n");
                    break;
                }
                printf("Message received: %s\n", msg);
                strcat(ans, "Wow, such message, so C-string: ");
                strcat(ans, msg);
                count = send(client_sock, ans, max_len * sizeof(char), 0);
                if (count < 0) {
                    fprintf(stderr, "Error responding to client.\n");
                }
            }
            shutdown(client_sock, SHUT_RDWR);
            close(client_sock);
        }
    }
}

