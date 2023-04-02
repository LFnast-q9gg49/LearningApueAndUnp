#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

int main() {
    int pd[2];
    pid_t pid;

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
        int fd;
        close(pd[1]); // close write
        dup2(pd[0], 0); // relocate read to standard input
        close(pd[0]);
        fd = open("/dev/null", O_RDWR);

        dup2(fd, 1);
        dup2(fd, 2); // only input

        execl("/usr/bin/mpg123", "mpg123", "-", nullptr);

        exit(0);
    }

    close(pd[0]); // close read

    // write to the pipe

    close(pd[1]);

    wait(nullptr);
    exit(0);
}

