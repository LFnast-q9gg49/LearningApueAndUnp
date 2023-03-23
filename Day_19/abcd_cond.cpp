// print abcd in terminal

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <cstring>

const int thread_max = 4;

int num = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int next(int a){
    if (a + 1 == 4){
        return 0;
    }
    return a + 1;
}



int main(){
    pthread_t tid[thread_max];
    int i;
    int err;

    auto print_handler = [](void *ptr) -> void *{
        auto param = *(int *) ptr;
        int c = 'a' + param;
        while (true) {
            pthread_mutex_lock(&mut);
            while(num != param){
                pthread_cond_wait(&cond, &mut); // unlock and wait
            }
            write(1, &c, 1);
            num = next(num);
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&mut);
        }

        pthread_exit(ptr);
    };

    for (i = 0; i < thread_max; i++){
        auto *arg = new int (i);

        err = pthread_create(tid+i, nullptr, print_handler, arg);
        if (err){
            fprintf(stderr, "create error : %s", strerror(err));
            exit(1);
        }
    }

    alarm(5);

    for (i = 0; i < thread_max; i++){
        pthread_join(*(tid+i), nullptr);
    }

    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    exit(0);
}
