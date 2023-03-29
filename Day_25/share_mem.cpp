// share memory demo

#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

const int mem_size = 1024;

int main() {
    int pid;
    void *map_ret;

    map_ret = mmap(nullptr, mem_size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (map_ret == MAP_FAILED){
        perror("mmap");
        exit(1);
    }

    pid = fork();
    if (pid < 0){
        perror("fork");
        munmap(map_ret, mem_size);
        exit(1);
    }

    if (pid == 0){
        // child
        strcpy((char *)map_ret, "Hello!");
        munmap(map_ret, mem_size);
        exit(0);
    }


    wait(nullptr);
    puts((char *)map_ret);
    munmap(map_ret, mem_size);
    exit(0);

}
