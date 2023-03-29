#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstring>

const int proc_max = 20;
const char *path = "./out";
const int LINESIZE = 1024;

void file_add(){
    FILE *fp;
    int fd;
    char get[LINESIZE];
    fp = fopen(path, "r+");
    if (fp == nullptr){
        perror("fopen()");
        exit(1);
    }

    fd = fileno(fp);

    lockf(fd, F_LOCK, 0);
    
    fgets(get, LINESIZE, fp);
    rewind(fp);// SEEK_SET
    sleep(1);

    fprintf(fp, "%d", atoi(get)+1);

    fflush(fp);// buffer
    lockf(fd, F_ULOCK, 0);
    fclose(fp);
}

int main(){
    pid_t pid;

    for (auto i = 0; i< proc_max; i++){
        pid = fork();
        if (pid < 0){
            fprintf(stderr, "fork error\n");
            exit(1);
        }
        if (pid == 0){
            file_add();
	    exit(0);
        }
    }

    for (auto i = 0; i < proc_max; i++){
        wait(nullptr);
    }


    exit(0);
}

