#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <cstring>
#include <unistd.h>

const int thread_max = 20;
const char *path = "/tmp/out";
const int LINESIZE = 1024;

void *file_add(void *){
    FILE *fp;
    char get[LINESIZE]; 
    fp = fopen(path, "r+");
    if (fp == nullptr){
        perror("fopen()");
        exit(1);
    }
    fgets(get, LINESIZE, fp);
    
    rewind(fp);// SEEK_SET
    fprintf(fp, "%d", atoi(get)+1);
    sleep(1);    
    fflush(fp);// buffer
    fclose(fp);
    
    pthread_exit(nullptr);
}

int main(){
    pthread_t tid[thread_max];
    int err;

    for (auto i = 0; i< thread_max; i++){
        err = pthread_create(tid+i, nullptr, &file_add, nullptr);
        if (err){
            fprintf(stderr, "create error : %s", strerror(err));
            exit(1);
        }
    }

    for (auto i = 0; i < thread_max; i++){
        pthread_join(tid[i], nullptr);
    }

    exit(0);
}
