#ifndef PROTO_H
#define PROTO_H

const char *key_path = "/etc/services";
const int key_project = 'q';

const int NAMESIZE = 32;

class proto {
public:
    struct msg_st{
        long mtype;
        char name[NAMESIZE];
        int math;
        int chinese;
    };
};


#endif

