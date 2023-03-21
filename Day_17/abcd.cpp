#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <cstring>
#include <csignal>

const int thread_max = 4;

static pthread_mutex_t mutex[thread_max];

static int next(int n){
    if (n + 1 == thread_max){
        return 0;
    }
    return n+1;
}

static void *thr_function(void *p){
    int n = *(int*)(p);
    int c = 'a' + n;
    while(true){
        pthread_mutex_lock(mutex+n);
        write(1, &c, 1);
        pthread_mutex_unlock(mutex+next(n));
    }

    pthread_exit(nullptr);
}

int main(){
    pthread_t tid[thread_max];
    int err;

    for (auto i = 0; i< thread_max; i++){

        pthread_mutex_init(mutex+i, nullptr);
        pthread_mutex_lock(mutex+i);

        int *arg = new int(i);
        err = pthread_create(tid+i, nullptr, &thr_function, arg);
        if (err){
            fprintf(stderr, "create error : %s", strerror(err));
            exit(1);
        }
    }

    pthread_mutex_unlock(mutex+0);

    alarm(5);

    for (auto i = 0; i < thread_max; i++){
        pthread_join(tid[i], nullptr);
    }

    pthread_mutex_destroy(mutex);

    exit(0);
}
