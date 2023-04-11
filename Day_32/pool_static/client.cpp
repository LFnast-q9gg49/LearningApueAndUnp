#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>
#include <cerrno>
#include <cstring>
#include "proto.h"

int main(int argc, char **argv){
    FILE *fp;
    int sd;
    long long stamp;
    struct sockaddr_in raddr{};

    if (argc < 2){
        fprintf(stderr, "Usage : ./client ip\n");
        exit(1);
    }
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0){
        perror("socket\n");
        exit(1);
    }

    // bind
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(server_port);
    inet_pton(AF_INET, argv[1], &raddr.sin_addr);

    if (connect(sd, (struct sockaddr *)&raddr, sizeof(raddr)) < 0){
        perror("connect");
        exit(1);
    }

    fp = fdopen(sd, "r+");
    if (fp == nullptr){
        fprintf(stdout, "socket file open error : %s\n", strerror(errno));
        // atexit
        exit(1);
    }

    if (fscanf(fp, fmt_stamp, &stamp) < 1){
        fprintf(stderr, "Bad Format\n");
        exit(1);
    } else{
        fprintf(stdout, "stamp = %lld\n", stamp);
    }

    fclose(fp);

    // recv();
    // close();

    exit(0);
}
