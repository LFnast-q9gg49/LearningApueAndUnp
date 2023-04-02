#include <cstdio>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "proto.h"

int main(){
    key_t key;
    int msgid;
    ssize_t ret;
    struct proto::msg_st rbuf{};

    key = ftok(key_path, key_project);
    if (key < 0){
        perror("key");
        exit(1);
    }

    msgid = msgget(key, IPC_CREAT|0600);
    if (msgid < 0){
        perror("msg get");
        exit(1);
    }

    while(true) {
        ret = msgrcv(msgid, &rbuf, sizeof(rbuf) - sizeof(long), 0, 0);
        if (ret < 0) {
            perror("msgrcv");
            exit(1);
        }
        fprintf(stdout, "name = %s\n", rbuf.name);
        fprintf(stdout, "math = %d\n", rbuf.math);
        fprintf(stdout, "chinese = %d\n", rbuf.chinese);
    }

    msgctl(msgid, IPC_RMID, nullptr);
    // return value

    exit(0);
}
