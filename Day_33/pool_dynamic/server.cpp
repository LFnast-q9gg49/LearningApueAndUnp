#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <csignal>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "proto.h"

const int MIN_SPARSE_SERVER = 5;
const int MAX_SPARSE_SERVER = 10;

const int CLIENT            = 20;

const int SIG_NOTIFY = SIGUSR2;

enum {
    STATE_IDLE = 0,
    STATE_BUSY,
};

struct server_st{
    pid_t pid;
    int state;
    // int reuse;
};

static struct server_st *server_pool;
static int idle_cnt , busy_cnt;

int main(){
    int sd, val = 1;
    pid_t pid;
    sigset_t set, old_set;
    struct sigaction sa{}, old_sa{};
    struct sockaddr_in laddr{};

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sa, &old_sa);

    sa.sa_handler = usr2_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_NOTIFY, &sa, &old_sa);

    sigemptyset(&set);
    sigaddset(&set, SIG_NOTIFY);
    sigprocmask(SIG_BLOCK, &set, &old_set);

    server_pool = (struct server_st *)mmap(nullptr,\
    sizeof(struct server_st) * CLIENT, PROT_READ|PROT_WRITE,\
    MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    if (server_pool == MAP_FAILED){
        perror("mmap()\n");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0){
        perror("socket()\n");
        exit(1);
    }

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0){
        perror("setsockopt()\n");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(server_port);
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
    if (bind(sd, (struct sockaddr *)&laddr, sizeof(laddr)) < 0){
        perror("bind()\n");
        exit(1);
    }

    if (listen(sd, 100) < 0){
        perror("listen()\n");
        exit(1);
    }

    for (auto i = 0; i < MIN_SPARSE_SERVER; i++){
        add_server();
    }

    while(true){
        sigsuspend(&old_set);

        scan_pool();

        if (idle_cnt > MAX_SPARSE_SERVER){
            for (auto i = 0; i < (idle_cnt - MAX_SPARSE_SERVER); i++){
                del_server();
            }
        }else if (idle_cnt < MIN_SPARSE_SERVER){
            for (auto i = 0; i < (MIN_SPARSE_SERVER - idle_cnt); i++){
                add_server();
            }
        }

        // printf the pool
        for (auto i = 0; i < CLIENT; i++){
            // idle busy and empty
        }
    }

    sigprocmask(SIG_SETMASK, &old_set, nullptr);

    exit(0);
}
