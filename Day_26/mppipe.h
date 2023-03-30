#ifndef MYPIPE_H
#define MYPIPE_H

#include <pthread.h>

const int PIPE_SIZE = 1024;

const int mypipe_reader = 0x00000001UL;
const int mypipe_writer = 0x00000010UL;


class mypipe {
public:
    static void *mypipe_create();
    static int mypipe_register(void *ptr, int opmap);
    static int mypipe_unregister(void *ptr, int opmap);
    static int mypipe_read(void *ptr, void *buf, size_t count);
    static int mypipe_write(void *ptr, const void *buf, size_t size);
    static int mypipe_destory(void *ptr);

    static int mypipe_read_byte_unlock(void *ptr, void *datap);

private:
    struct my_pipe_st{
        int head;
        int tail;
        char data[PIPE_SIZE];
        int data_size; // valid char
        int reader_count;
        int writer_count;
        pthread_mutex_t mut;
        pthread_cond_t cond;
    };

};


#endif //MYPIPE_H

