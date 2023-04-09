// use http1.0

#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>

const int bufsize = 1024;

int main(int argc, char **argv){
    FILE *fp;
    int sd, get;
    struct sockaddr_in raddr{};
    char rbuf[bufsize];

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
    raddr.sin_port = htons(80);
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

    fprintf(fp, "GET /1.png\r\n\r\n");
    fflush(fp);

    while(true){
        get = fread(rbuf, 1, bufsize, fp);
        fwrite(rbuf, 1, get, stdout);
    }

    fclose(fp);

    // recv();
    // close();

    exit(0);
}

