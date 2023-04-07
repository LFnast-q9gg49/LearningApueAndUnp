#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "proto.h"

int main(){
    int sd;
    struct sockaddr_in sockaddrIn{};

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0){
        perror("socket\n");
        exit(1);
    }

    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(server_port);
    inet_pton(AF_INET, "0.0.0.0", &sockaddrIn.sin_addr);


    if (bind(sd, (struct sockaddr *)&sockaddrIn, sizeof(sockaddrIn))<0){
        perror("bind");
        exit(1);
    }

    // second value is the number of half-connect pool. Now is full connect num.
    if (listen(sd, 200) < 0){
        perror("listen");
        exit(1);
    }
    
    accept();

    send();

    close();
}
