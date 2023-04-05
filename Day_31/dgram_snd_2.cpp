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
    int size = sizeof(struct msg_st) + strlen(argv[2]);
    auto *message_p = (struct msg_st*)malloc(size);
    if (message_p == nullptr){
        perror("malloc");
        exit(1);
    }
    struct sockaddr_in remote_add{};

    if (argc < 3){
        fprintf(stderr, "Usage : ./dgram_snd IP NAME");
        exit(1);
    }

    if (strlen(argv[2]) > name_max){
        fprintf(stderr, "name too long");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0){
        perror("socket");
        exit(1);
    }
    // ignore bind(), port auto distribution

    strcpy((char *)message_p->name, argv[2]);
    message_p->math = htonl(rand()%100);
    message_p->chinese = htonl(rand()%100);

    remote_add.sin_family = AF_INET;
    remote_add.sin_port = htons(rcv_port);
    inet_pton(AF_INET, argv[1], &remote_add.sin_addr);

    if (sendto(sd, message_p, size, 0, (sockaddr *)&remote_add, sizeof(remote_add)) < 0){
        perror("send");
        exit(1);
    }

    fprintf(stdout, "finished\n");

    close(sd);
    exit(0);
}

