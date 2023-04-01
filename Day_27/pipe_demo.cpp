#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <wait.h>

const int buf_size = 10;

int main() {
    int pd[2];
    pid_t pid;
    char buf[buf_size];

    if (pipe(pd) < 0){
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid < 0){
        perror("fork");
        exit(1);
    }

    if (pid == 0){
        // child
        close(pd[1]); // close write
        int len;

        len = read(pd[0], buf, buf_size);
        write(1, buf, len);

        close(pd[0]);

        exit(0);
    }
    
    close(pd[0]); // close read

    write(pd[1], "Hello !", 7);
    close(pd[1]);

    wait(nullptr);
    exit(0);
}

