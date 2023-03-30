#include "mypipe.h"
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int next(int n){
    if (n + 1 < PIPE_SIZE){
        return n+1;
    }
    return PIPE_SIZE;
}

int mypipe::mypipe_read_byte_unlock(void *ptr, void *datap) {
    auto me = (struct my_pipe_st *)ptr;
    auto str = (char *)datap;

    if (me->data_size <= 0){
        return -1;
    }

    *str = me->data[me->head];
    me->head = next(me->head);
    me->data_size--;

    return 0;
}

int mypipe::mypipe_register(void *ptr, int opmap) {
    /* if error */
    auto me = new my_pipe_st();

    pthread_mutex_lock(&me->mut);

    if (opmap & mypipe_reader){
        me->reader_count++;
    }
    if (opmap & mypipe_writer){
        me->writer_count++;
    }

    while(me->writer_count <= 0 || me->reader_count <= 0){
        pthread_cond_wait(&me->cond, &me->mut);
    }

    pthread_mutex_unlock(&me->mut);
    pthread_cond_broadcast(&me->cond);

    return 0;
}

int mypipe::mypipe_unregister(void *ptr, int opmap) {
    /* if error */
    auto me = new my_pipe_st();

    pthread_mutex_lock(&me->mut);

    if (opmap & mypipe_reader){
        me->reader_count--;
    }
    if (opmap & mypipe_writer){
        me->writer_count--;
    }

    pthread_mutex_unlock(&me->mut);
    pthread_cond_broadcast(&me->cond);

    return 0;
}

void *mypipe::mypipe_create() {
    auto me = new my_pipe_st();
    me->head = 0;
    me->tail = 0;
    me->data_size = 0;
    me->reader_count = 0;
    me->writer_count = 0;
    pthread_mutex_init(&me->mut, nullptr);
    pthread_cond_init(&me->cond, nullptr);
    return me;
}

int mypipe::mypipe_read(void *ptr, void *buf, size_t count) {
    int i;
    auto me = (struct my_pipe_st *)ptr;
    auto str = (char *)buf;

    pthread_mutex_lock(&me->mut);
    while (me->data_size <= 0 && me->writer_count > 0){
        pthread_cond_wait(&me->cond, &me->mut);
    }

    if (me->data_size <= 0 && me->writer_count <= 0){
        pthread_mutex_unlock(&me->mut);
        return 0;
    }

    for (i = 0; i < count; i++){
        if (mypipe::mypipe_read_byte_unlock(me, str+i) < 0){
            break;
        }
    }
    pthread_mutex_unlock(&me->mut);
    pthread_cond_broadcast(&me->cond);

    return i;
}

int mypipe::mypipe_write(void *ptr, const void *buf, size_t size) {
    auto me = (struct my_pipe_st *)ptr;

    pthread_mutex_lock(&me->mut);
    while (me->data_size == PIPE_SIZE && me->reader_count > 0){
        pthread_cond_wait(&me->cond, &me->mut);
    }

    if (me->data_size == PIPE_SIZE && me->writer_count <= 0){
        return 0;
    }

    pthread_mutex_unlock(&me->mut);

    return 0;
}

int mypipe::mypipe_destory(void *ptr) {
    auto me = (struct my_pipe_st *)ptr;

    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);

    delete me;
    return 0;
}






