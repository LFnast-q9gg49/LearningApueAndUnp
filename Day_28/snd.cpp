#include <cstdio>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>

#include "proto.h"

int main(){
    key_t key;
    int msgid, ret;
    struct proto::msg_st sbuf{};


    key = ftok(key_path, key_project);
    if (key < 0){
        perror("key");
        exit(1);
    }

    msgid = msgget(key, 0);
    if (msgid < 0){
        perror("msg get");
        exit(1);
    }

    sbuf.mtype = 1;
    strcpy(sbuf.name, "AI");
    sbuf.math = rand()%100;
    sbuf.chinese = rand()%100;

    ret = msgsnd(msgid, &sbuf, sizeof(sbuf) - sizeof(long), 0);
    if (ret < 0){
        perror("snd");
        exit(1);
    }

    puts("ok");

    exit(0);
}
