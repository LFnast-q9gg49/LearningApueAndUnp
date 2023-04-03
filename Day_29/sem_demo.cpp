#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

const int proc_max = 20;
const char *path = "./out";
const int LINESIZE = 1024;

static void P(int semid){
    struct sembuf op{};
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;
    while (semop(semid, &op, 1) < 0){
        if (errno != EINTR || errno != EAGAIN){
            perror("semop()");
            exit(1);
        }
    }
}

static void V(int semid){
    struct sembuf op{};
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;
    while (semop(semid, &op, 1) < 0){
        if (errno != EINTR || errno != EAGAIN){
            perror("semop()");
            exit(1);
        }
    }
}

void file_add(int semid){
    FILE *fp;
    char get[LINESIZE];
    fp = fopen(path, "r+");
    if (fp == nullptr){
        perror("fopen()");
        exit(1);
    }

    // P
    P(semid);

    fgets(get, LINESIZE, fp);
    rewind(fp);// SEEK_SET
    sleep(1);

    fprintf(fp, "%d", atoi(get)+1);
    fflush(fp);// buffer

    // V
    V(semid);

    fclose(fp);
}

int main(){
    pid_t pid;
    int i, semid;

    semid = semget(IPC_PRIVATE, 1, 0600);
    if (semid < 0){
        perror("semget");
        exit(1);
    }

    semctl(semid, 0, SETVAL, 1);
    // error

    for (i = 0; i< proc_max; i++){
        pid = fork();
        if (pid < 0){
            fprintf(stderr, "fork error\n");
            exit(1);
        }
        if (pid == 0){
            file_add(semid);
            exit(0);
        }
    }

    for (i = 0; i < proc_max; i++){
        wait(nullptr);
    }

    semctl(semid, 0,  IPC_RMID);

    exit(0);
}

