// Sys V IPC shm

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <cstring>

const int SHM_SIZE = 1024;

int main(){
    pid_t pid;
    int shm_id;
    char *ptr;

    shm_id = shmget(IPC_PRIVATE, SHM_SIZE, 0600);
    if (shm_id < 0){
        perror("shmget()");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }else if (pid == 0){
        // child write
        ptr = (char *)shmat(shm_id, nullptr, 0);
        if (ptr == (void *)-1){
            perror("shmat");
            exit(1);
        }
        strcpy(ptr, "hello");
        shmdt(ptr);
        exit(0);
    }

    wait(nullptr);
    ptr = (char *)shmat(shm_id, nullptr, 0);
    if (ptr == (void *)-1){
        perror("shmat");
        exit(1);
    }
    fprintf(stdout, "%s\n", ptr);
    shmdt(ptr);

    shmctl(shm_id, IPC_RMID, nullptr);

    exit(0);
}
