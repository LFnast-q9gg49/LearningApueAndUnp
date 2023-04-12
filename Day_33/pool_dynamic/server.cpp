#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <sys/socket.h>
#include <csignal>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "proto.h"

const int MIN_SPARSE_SERVER = 5;
const int MAX_SPARSE_SERVER = 10;

static int sd;

const int CLIENT = 20;

const int bufsize = 1024;
const int ipstrsize = 40;

const int SIG_NOTIFY = SIGUSR2;

enum {
    STATE_IDLE = 0,
    STATE_BUSY,
};

struct server_st {
    pid_t pid; // -1 is null
    int state;
    // int reuse;
};

static struct server_st *server_pool;
static int idle_cnt, busy_cnt;

static void server_job(int pos) {
    pid_t ppid;
    int len;
    ppid = getppid();
    struct sockaddr_in raddr{};
    socklen_t raddr_len;
    time_t stamp;
    char linebuf[bufsize];
    char ipstr[ipstrsize];

    int client_sd;

    while (true) {
        server_pool[pos].state = STATE_IDLE;
        kill(ppid, SIG_NOTIFY);

        client_sd = accept(sd, (struct sockaddr *) &raddr, &raddr_len);
        if (client_sd < 0) {
            perror("client\n");
            exit(1);
        }

        server_pool[pos].state = STATE_BUSY;
        kill(ppid, SIG_NOTIFY);

        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, ipstrsize);
        fprintf(stdout, "[%d]client:%s%d\n", getpid(), ipstr, ntohs(raddr.sin_port));

        stamp = time(nullptr);
        len = snprintf(linebuf, bufsize, fmt_stamp, stamp);
        send(client_sd, linebuf, len, 0);
        sleep(5); // stimulate the real process time
	close(client_sd); 
    }
}

// return 0 (success) or -1 (failed)
static int add_server() {
    pid_t pid;
    int slot;

    if (idle_cnt + busy_cnt >= CLIENT) {
        return -1;
    }
    for (slot = 0; slot < CLIENT; slot++) {
        if (server_pool[slot].pid == -1) {
            break;
        }
    }
    server_pool[slot].state = STATE_IDLE;
    pid = fork();
    if (pid < 0) {
        perror("fork()\n");
        exit(1);
    } else if (pid == 0) {
        server_job(slot);
        exit(0);
    }

    server_pool[slot].pid = pid;
    idle_cnt++;
    return 0;

}

// return 0 (success) or -1 (failed)
static int del_server() {
    int i;
    if (idle_cnt == 0) {
        return -1;
    }
    for (i = 0; i < CLIENT; i++) {
        if (server_pool[i].pid != -1 && server_pool[i].state == STATE_IDLE) {
            kill(server_pool[i].pid, SIGTERM);
            server_pool[i].pid = -1;
            idle_cnt--;
        }
    }
    return 0;
}

static void scan_pool() {
    int busy = 0, idle = 0;
    for (auto i = 0; i < CLIENT; i++) {
        if (server_pool[i].pid == -1) {
            continue;
        }
        if (kill(server_pool[i].pid, 0)) { // test progress
            server_pool[i].pid = -1;
            continue;
        }
        if (server_pool[i].state == STATE_IDLE) {
            idle++;
        } else if (server_pool[i].state == STATE_BUSY) {
            busy++;
        } else {
            fprintf(stderr, "Unknown state\n");
            abort(); // or use _exit
        }
    }

    idle_cnt = idle;
    busy_cnt = busy;
}

int main() {
    int val = 1;
    sigset_t set, old_set;
    struct sigaction sa{}, old_sa{};
    struct sockaddr_in laddr{};

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sa, &old_sa);

    auto usr2_handler = [](int s) -> void {
        return;
    };

    sa.sa_handler = usr2_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_NOTIFY, &sa, &old_sa);

    sigemptyset(&set);
    sigaddset(&set, SIG_NOTIFY);
    sigprocmask(SIG_BLOCK, &set, &old_set);

    server_pool = (struct server_st *) mmap(nullptr, \
    sizeof(struct server_st) * CLIENT, PROT_READ | PROT_WRITE, \
    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    for (auto i = 0; i < CLIENT; i++) {
        server_pool[i].pid = -1;
    }

    if (server_pool == MAP_FAILED) {
        perror("mmap()\n");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket()\n");
        exit(1);
    }

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
        perror("setsockopt()\n");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(server_port);
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
    if (bind(sd, (struct sockaddr *) &laddr, sizeof(laddr)) < 0) {
        perror("bind()\n");
        exit(1);
    }

    if (listen(sd, 100) < 0) {
        perror("listen()\n");
        exit(1);
    }


    for (auto i = 0; i < MIN_SPARSE_SERVER; i++) {
        add_server();
    }

    while (true) {
        sigsuspend(&old_set);

        scan_pool();

        if (idle_cnt > MAX_SPARSE_SERVER) {
            for (auto i = 0; i < (idle_cnt - MAX_SPARSE_SERVER); i++) {
                del_server();
            }
        } else if (idle_cnt < MIN_SPARSE_SERVER) {
            for (auto i = 0; i < (MIN_SPARSE_SERVER - idle_cnt); i++) {
                add_server();
            }
        }

        // printf the pool
        for (auto i = 0; i < CLIENT; i++) {
            // idle busy and empty
            if (server_pool[i].pid == -1){
                putchar(' ');
            }else if (server_pool[i].state == STATE_IDLE){
                putchar('.');
            }else{
                putchar('x');
            }
        }
        putchar('\n');
    }

    sigprocmask(SIG_SETMASK, &old_set, nullptr);

    exit(0);
}
