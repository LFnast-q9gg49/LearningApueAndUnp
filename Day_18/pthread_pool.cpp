#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <cstring>

const int LEFT = 30000000;
const int RIGHT = 30000200;
const int thread_number = 4;

static int task = 0; // 0 is none, -1 is over, >0 is task
static pthread_mutex_t mut_task = PTHREAD_MUTEX_INITIALIZER;

static void *thr_primer(void *p){

    int j, mark;
    while(true) {
        pthread_mutex_lock(&mut_task);
        while (task == 0) {
            pthread_mutex_unlock(&mut_task);
            sched_yield();
            pthread_mutex_lock(&mut_task);
        }
        if (task == -1){
            pthread_mutex_unlock(&mut_task);
            break;
        }
        int i = task;
        task = 0;
        pthread_mutex_unlock(&mut_task);

        mark = 1;
        for (j = 2; j < i / 2; j++) {
            if (i % j == 0) {
                mark = 0;
                break;
            }
        }
        if (mark) {
            fprintf(stdout, "thread [%d] : %d is a primer\n", *(int *)p, i);
        }
    }
    pthread_exit(p);
}

int main(){
    int i;
    pthread_t tid[thread_number];
    int err;
    void *ptr;

    for (i = 0; i < thread_number; i++){
        int* arg = new int(i);
        err = pthread_create(tid + i, nullptr, &thr_primer, arg);
        if (err) {
            fprintf(stderr, "create error : %s", strerror(err));
            exit(1);
        }
    }

    for (i = LEFT; i <= RIGHT;  i++){
        pthread_mutex_lock(&mut_task);

        while(task != 0){
            pthread_mutex_unlock(&mut_task);
            sched_yield();
            pthread_mutex_lock(&mut_task);
        }
        task = i;
        pthread_mutex_unlock(&mut_task);
    }

    pthread_mutex_lock(&mut_task);
    while (task != 0){
        pthread_mutex_unlock(&mut_task);
        sched_yield();
        pthread_mutex_lock(&mut_task);
    }
    task = -1;
    pthread_mutex_unlock(&mut_task);

    for (i = 0; i < thread_number; i++){
        pthread_join(tid[i], &ptr);
        delete (int *)ptr;
    }

    pthread_mutex_destroy(&mut_task);

    exit(0);
}
