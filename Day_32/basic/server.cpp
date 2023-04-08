#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>
#include <cstring>
#include "proto.h"

const int ipstrsize = 40;
const int bufsize = 1024;

static void server_job(int tmp_sd){
    char buf[bufsize];
    size_t len;

    sprintf(buf, fmt_stamp, (long long)time(nullptr));
    len = strlen(buf);

    if (send(tmp_sd, buf, len, 0) < 0){
        perror("send");
        exit(1);
    }
}

int main(){
    int sd, tmp_sd;
    socklen_t raddr_len;
    struct sockaddr_in laddr{}, raddr{};
    char ipstr[ipstrsize];

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0){
        perror("socket\n");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(server_port);
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);


    if (bind(sd, (struct sockaddr *)&laddr, sizeof(laddr)) < 0){
        perror("bind");
        exit(1);
    }

    // second value is the number of half-connect pool. Now is full connect num.
    if (listen(sd, 200) < 0){
        perror("listen");
        exit(1);
    }

    raddr_len = sizeof(raddr);
    while(true) {
        tmp_sd = accept(sd, (struct sockaddr *) &raddr, &raddr_len);
        if ( tmp_sd < 0){
            perror("accept");
            exit(1);
        }
        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, ipstrsize);
        fprintf(stdout, "Client:%s:%d\n", ipstr, ntohs(raddr.sin_port));

        server_job(tmp_sd);

        close(tmp_sd);
    }

    /*
    close(sd);

    exit(0);
    */
}
