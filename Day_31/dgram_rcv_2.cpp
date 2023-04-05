#include <cstdio>
#include <cstdlib>
#include <sys/socket.h> // SOCK_DGRAM
#include <netinet/in.h> // man 7 ip
#include <arpa/inet.h> // inet_pton
#include "proto.h"

const int IPSTRSIZE = 1024;

int main() {
    int sd;
    struct sockaddr_in local_add{}, remote_add{};
    int size = sizeof (struct msg_st) + name_max - 1;
    auto *message_p = (struct msg_st*)malloc(size);
    if (message_p == nullptr){
        perror("malloc");
        exit(1);
    }
    char ipstr[IPSTRSIZE];

    /*!!!*/
    socklen_t remote_add_len = sizeof(remote_add);

    sd = socket(AF_INET, SOCK_DGRAM, 0/*IPPROTO_UDP*/);

    if (sd < 0){
        perror("socket");
        exit(1);
    }

    local_add.sin_family = AF_INET;
    local_add.sin_port = htons(rcv_port);
    inet_pton(AF_INET, "192.168.43.244", &local_add.sin_addr);

    if (bind(sd, (sockaddr *)(&local_add), sizeof(local_add)) < 0){
        perror("bind");
        exit(1);
    }

    while (true){
        recvfrom(sd, message_p, size, 0, (sockaddr *)(&remote_add), &remote_add_len); // is not like stream 1 to 1, need to discriminate

        inet_ntop(AF_INET, &remote_add.sin_addr, ipstr, IPSTRSIZE);
	fprintf(stdout, "%s\n", (char *)message_p->name);
        fprintf(stdout, "---MESSAGE FROM %s:%d---\n", ipstr, ntohs(remote_add.sin_port));
        fprintf(stdout, "name is %s\nmath score is %d\nchinese score is %d\n", message_p->name, ntohl(message_p->math), ntohl(message_p->chinese));
    }

    // close(sd);
    // exit(0);
}

