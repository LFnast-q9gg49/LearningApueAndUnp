#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include "mytbf.h"

const int CPS 	= 10;
const int SIZE 	= 1024;
const int BURST = 100;

int main(int argc, char **argv){
    int sfd = -1, dfd = 1;
    char buf[SIZE];
    int len = -1, ret = -1, pos = -1;
    mytbf_t *tbf;
    int size = 0;

    auto error_handler = []() -> void{
        fprintf(stderr, "error occur %s\n", strerror(errno));
        exit(1);
    };

    if (argc < 2){
        error_handler();
    }

    tbf = mytbf_init(CPS, BURST);
    if (tbf == nullptr){
        perror("init error");
        exit(1);
    }

    do{
        sfd = open(argv[1], O_RDONLY);
        if (sfd < 0){
            if (errno != EINTR){
                error_handler();
            }
        }
    }while(sfd < 0);


    while(true){
        size = mytbf_fetchtoken(tbf, SIZE);

        while((len = read(sfd, buf, size)) < 0){
            if (errno == EINTR){
                continue;
            }
            perror("read()");
            break;
        }

        if (len == 0){
            break;
        }
        if (size - len > 0){
            mytbf_returntoken(tbf, size-len);
        }
        pos = 0;

        while(len > 0){
            ret = write(dfd, buf+pos, len);
            if (ret < 0){
                if (errno == EINTR){
                    continue;
                }
                error_handler();
            }
            pos += ret;
            len -= ret;
        }
    }
    close(sfd);

    mytbf_destroy(tbf);

    exit(0);
}
