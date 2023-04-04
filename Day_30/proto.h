#ifndef PROTO_H
#define PROTO_H

const int rcv_port = 2023;
const int name_size = 17;

struct msg_st{
    char name[name_size];
    uint32_t math;
    uint32_t chinese;
}__attribute__((packed));


#endif

