#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <cstring>
#include <unistd.h>
#include "mysem.h"

const int LEFT = 30000000;
const int RIGHT = 30000200;
const int thread_number = RIGHT - LEFT + 1;

const int N = 4;

static void *thr_primer(void *p);

static mysem_t *sem;

int main(){
    int i;
    pthread_t tid[thread_number];
    int err;

    sem = mysem::mysem_init(N);
    if (sem == nullptr){
        fprintf(stderr, "init error");
        exit(1);
    }

    for (i = LEFT; i <= RIGHT; i++){
        mysem::mysem_sub(sem, 1);
        int *arg = new int(i);
        err = pthread_create(tid + i - LEFT, nullptr, &thr_primer, arg);
        if (err) {
            fprintf(stderr, "create error : %s", strerror(err));
            exit(1);
        }
    }

    for (i = LEFT; i <= RIGHT; i++){
        pthread_join(tid[i-LEFT], nullptr);
    }

    mysem::mysem_destory(sem);

    exit(0);
}

static void *thr_primer(void *p){
    int i = *(int *)p;

    int j, mark;

    mark = 1;
    for (j = 2; j < i/2; j++){
        if (i % j == 0){
            mark = 0;
            break;
        }
    }
    if (mark){
        fprintf(stdout, "%d is a primer\n", i);
    }

    sleep(5); // behold the pid
    mysem::mysem_add(sem, 1);

    pthread_exit(nullptr);
}


