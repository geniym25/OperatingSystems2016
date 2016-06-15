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

int main(int argc, char **argv) {
    const size_t max_len = 1024;
    const uint16_t port = 4242;
    int sock;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t client_len;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

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

    client_len = sizeof(struct sockaddr_in);
    ssize_t count;
    char *msg = (char *) malloc(max_len * sizeof(char));
    char *ans = (char *) malloc(max_len * sizeof(char));
    while (1) {
        bzero(msg, max_len * sizeof(char));
        bzero(ans, max_len * sizeof(char));
        count = recvfrom(sock, msg, max_len * sizeof(char), 0, (struct sockaddr *) &cli_addr, &client_len);
        if (count < 0) {
            fprintf(stderr, "Error reading message.\n");
        }
        if (strcmp(msg, "srv_exit") == 0) {
            printf("*** Received shutdown message.\n");
            close(sock);
            return 0;
        }
        printf("Message received: %s\n", msg);
        strcat(ans, "Wow, such message, so C-string: ");
        strcat(ans, msg);
        count = sendto(sock, ans, max_len * sizeof(char), 0, (struct sockaddr *) &cli_addr, client_len);
        if (count < 0) {
            fprintf(stderr, "Error responding to client.\n");
        }
    }
}

