#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "proto.h"

int main(int argc, char **argv) {

    int sd;
    int size = sizeof(struct msg_st) + strlen(argv[1]);
    auto *message_p = (struct msg_st*)malloc(size);

    if (message_p == nullptr){
        perror("malloc");
        exit(1);
    }
    struct sockaddr_in remote_add{};

    if (argc < 2){
        fprintf(stderr, "Usage : ./dgram_snd NAME\n");
        exit(1);
    }


    if (strlen(argv[1]) > name_max){
        fprintf(stderr, "name too long\n");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0){
        perror("socket");
        exit(1);
    }
    // ignore bind(), port auto distribution

    struct ip_mreqn mreq{};
    inet_pton(AF_INET, GROUP, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("eth0");
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq)) < 0){
        perror("setsock");
        exit(1);
    }

    strcpy((char *)message_p->name, argv[1]);
    message_p->math = htonl(rand()%100);
    message_p->chinese = htonl(rand()%100);

    remote_add.sin_family = AF_INET;
    remote_add.sin_port = htons(rcv_port);
    inet_pton(AF_INET, "0.0.0.0", &remote_add.sin_addr);

    if (sendto(sd, message_p, size, 0, (sockaddr *)&remote_add, sizeof(remote_add)) < 0){
        perror("send");
        exit(1);
    }

    fprintf(stdout, "finished\n");

    close(sd);
    exit(0);
}


