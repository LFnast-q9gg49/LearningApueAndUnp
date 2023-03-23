#include <pthread.h>
#include "mysem.h"
#include <cstdio>

struct mysem_st{
    int value;
    pthread_mutex_t mut;
    pthread_cond_t cond;
};

mysem_t *mysem::mysem_init(int init_val) {
    auto *me = new struct mysem_st();

    me -> value = init_val;
    pthread_mutex_init(&me -> mut, nullptr);
    pthread_cond_init(&me -> cond, nullptr);

    return me;
}

int mysem::mysem_add(mysem_t *ptr, int n) {
    auto *me = (struct mysem_st *)ptr;

    pthread_mutex_lock(&me -> mut);
    fprintf(stdout, "add start and now value is %d \n", me->value);
    me->value += n;
    pthread_cond_broadcast(&me->cond); // maybe after return more than one thread can get resource
    pthread_mutex_unlock(&me -> mut);

    return n;
}

int mysem::mysem_sub(mysem_t *ptr, int n) {
    auto *me = (struct mysem_st *)ptr;

    pthread_mutex_lock(&me -> mut);
    fprintf(stdout, "sub start and now value is %d \n", me->value);
    while(me->value < n){
        pthread_cond_wait(&me -> cond, &me -> mut);
    }
    me->value -= n;
    pthread_mutex_unlock(&me -> mut);

    return n;
}

int mysem::mysem_destory(mysem_t *ptr) {
    auto *me = (struct mysem_st *)ptr;
    pthread_mutex_destroy(&me -> mut);
    pthread_cond_destroy(&me -> cond);

    delete me;

    return 0;
}

