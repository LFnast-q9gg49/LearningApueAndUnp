#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>

const char *TTY1 = "/dev/tty1";
const char *TTY2 = "/dev/tty2";

const int BUFSIZE = 100;

enum {
    STATE_R = 1,
    STATE_W,
    STATE_Ex,
    STATE_T,
};

struct fsm_st{
    int state;
    int sfd;
    int dfd;
    int len;
    int pos;
    char buf[BUFSIZE];
    char *err_str;
};

int fsm_driver(struct fsm_st *fsm){
    int ret;
    switch (fsm->state) {
        case STATE_R:
            fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
            if (fsm->len == 0){
                fsm->state = STATE_T;
            }else if (fsm->len < 0){
                if (errno == EAGAIN){
                    fsm->state = STATE_R;
                }else{
                    fsm->err_str = "read";
                    fsm->state = STATE_Ex;
                }
            } else{
                fsm->pos = 0;
                fsm->state = STATE_W;
            }
            break;
        case STATE_W:
            ret = write(fsm->dfd, fsm->buf + fsm->pos, fsm->len);
            if (ret < 0){
                if (errno == EAGAIN){
                    fsm->state = STATE_W;
                }else{
                    fsm->err_str = "write";
                    fsm->state = STATE_Ex;
                }
            } else{
                fsm->pos += ret;
                fsm->len -= ret;
                if (fsm->len == 0){
                    fsm->state = STATE_R;
                }else{
                    fsm->state = STATE_W;
                }
            }
            break;
        case STATE_Ex:
            // perror
            perror(fsm->err_str);

            fsm->state = STATE_T;
            break;
        case STATE_T:
            abort();

        default:
            break;
    }
}

int relay(int fd_1, int fd_2){
    int fd_1_save;
    int fd_2_save;

    struct fsm_st fsm12{}, fsm21{};

    fd_1_save = fcntl(fd_1, F_GETFL);
    fcntl(fd_1, F_SETFL, fd_1_save|O_NONBLOCK);

    fd_2_save = fcntl(fd_2, F_GETFL);
    fcntl(fd_1, F_SETFL, fd_2_save|O_NONBLOCK);

    fsm12.state = STATE_R;
    fsm12.sfd = fd_1;
    fsm12.dfd = fd_2;

    fsm21.state = STATE_R;
    fsm21.sfd = fd_2;
    fsm21.dfd = fd_1;

    while(fsm12.state != STATE_T || fsm21.state != STATE_T){
        fsm_driver(&fsm12);
        fsm_driver(&fsm21);
    }

    fcntl(fd_1, F_SETFL, fd_1_save);
    fcntl(fd_2, F_SETFL, fd_2_save);
}

int main(){
    int fd_1, fd_2;
    fd_1 = open(TTY1, O_RDWR);
    if (fd_1 < 0){
        perror("open");
        exit(1);
    }
    write(1, "TTY1\n", 5);
    fd_2 = open(TTY2, O_RDWR);
    if (fd_2 < 0){
        perror("open");
        exit(1);
    }
    write(1, "TTY2\n", 5);

    relay(fd_1, fd_2);

    close(fd_1);
    close(fd_2);
    exit(0);
}
