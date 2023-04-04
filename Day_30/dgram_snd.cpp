#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "proto.h"

int main(int argc, char **argv) {
    int sd;
    struct msg_st message{};
    struct sockaddr_in remote_add{};

    if (argc < 2){
        fprintf(stderr, "Usage ...");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0){
        perror("socket");
        exit(1);
    }
    // ignore bind(), auto port distribution

    strcpy(message.name, "TEST");
    message.math = htonl(rand()%100);
    message.chinese = htonl(rand()%100);

    remote_add.sin_family = AF_INET;
    remote_add.sin_port = htons(rcv_port);
    inet_pton(AF_INET, argv[1], &remote_add.sin_addr);

    if (sendto(sd, &message, sizeof(message), 0, (sockaddr *)&remote_add, sizeof(remote_add)) < 0){
        perror("send");
        exit(1);
    }

    fprintf(stdout, "finished");

    close(sd);
    exit(0);
}

