//
// Created by vks on 22.05.16.
//
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <zconf.h>

int main(int argc, char **argv) {
    const size_t max_len = 1024;
    const uint16_t port = 4242;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int sock;

    if (argc == 1) {
        fprintf(stderr, "No hostname given.\n");
        return 1;
    }
    if (argc == 2) {
        server = gethostbyname(argv[1]);
        if (server == NULL) {
            fprintf(stderr, "Invalid hostname.\n");
            return 1;
        }
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        fprintf(stderr, "Error creating socket!\n");
        return 1;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr,
          (size_t) server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) {
        fprintf(stderr, "Could not establish connection to %s.\n", argc == 2 ? argv[1] : argv[2]);
        return 1;
    }
    char *msg = (char *) malloc(max_len * sizeof(char));
    while (strcmp(gets(msg), "exit") != 0) {
        ssize_t count = send(sock, msg, strlen(msg) * sizeof(char), 0);
        if (count < 0) {
            fprintf(stderr, "Error writing to socket.\n");
        }
        count = recv(sock, msg, max_len * sizeof(char), MSG_WAITALL);
        if (count < 0) {
            fprintf(stderr, "Error receiving answer.\n");
        } else {
            printf("*** Server response: %s\n", msg);
        }
        bzero(msg, max_len * sizeof(char));
    }
    printf("*** Received shutdown message.\n");
    close(sock);
    return 0;
}
