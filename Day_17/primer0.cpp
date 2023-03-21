#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

const int LEFT = 30000000;
const int RIGHT = 30000200;
const int thread_number = RIGHT - LEFT + 1;

static void *thr_primer(void *p);

int main(){
    int i;
    pthread_t tid[thread_number];
    int err;

    for (i = LEFT; i <= RIGHT; i++){
        err = pthread_create(tid + i - LEFT, nullptr, &thr_primer, (void *)i);
        if (err) {
            fprintf(stderr, "create error : %s", strerror(err));
            exit(1);
        }
    }

    for (i = LEFT; i <= RIGHT; i++){
        pthread_join(tid[i-LEFT], nullptr);
    }

    exit(0);
}

static void *thr_primer(void *p){
    int i = (int)p;

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
    pthread_exit(nullptr);
}

