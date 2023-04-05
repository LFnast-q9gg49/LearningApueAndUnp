#ifndef PROTO_H
#define PROTO_H

const int rcv_port = 2023;

const char * GROUP = "224.2.2.2";

const int name_max = 512/*udp recommend*/ - 8/*two uint32_t*/ - 8 /*udp header*/;

struct msg_st{
    uint32_t math;
    uint32_t chinese;
    uint8_t name[1];
}__attribute__((packed));


#endif


