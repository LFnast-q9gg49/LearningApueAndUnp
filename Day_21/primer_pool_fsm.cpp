// use 5 pthreads to calculate primes between 1000 and 2000
// and write them to the file
// just an exercise of FSM

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <cstring>

static const char       *path       = "./fsm_prime";
static const int        pthread_max = 5;
static pthread_mutex_t  mut         = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t   cond        = PTHREAD_COND_INITIALIZER;

enum {
    STATE_O, // open file
    STATE_G, // get num
    STATE_C, // calculate;
    STATE_W, // write file;
    STATE_E, // exit
    STATE_T, // terminate;
};

class prime_fsm{
public:
    static void *fsm_init(){
        auto me = new struct fsm();
        return me;
    }

    static int fsm_getstate(void *ptr){
        auto *me = (struct fsm *)ptr;
        return me->state;
    }

    static void fsm_setthrid(void *ptr, int id){
        auto *me = (struct fsm *)ptr;
        me->thr_id = id;
    }

    static int fsm_driver(void *ptr){
        auto *me = (struct fsm *)ptr;
        int j;
        me->mark = 1;
        switch (me->state) {
            case STATE_O:

                me->file_ptr = fopen(path, "a");

                if (me->file_ptr == nullptr){
                    me->state = STATE_E;
                }

                me->state = STATE_G;

                break;

            case STATE_G:

                // get number from pool
                break;

            case STATE_C:

                // better use miller-rabin prime, here I just use the most simple way
                for (j = 2; j < me->calc_num / 2; j++){
                    if (me->calc_num % j == 0) {
                        me->mark = 0;
                        break;
                    }
                }

                if (me->mark){

                    // prime get
                    me->state = STATE_W;
                } else{

                    // this num is not a prime
                    me->state = STATE_G;
                }

                break;
            case STATE_W:

                fprintf(me->file_ptr, "%d\t", me->calc_num);
                me->state = STATE_G;

                break;
            case STATE_E:

                me->state = STATE_T;

                break;
            case STATE_T:
                break;
            default:
                break;
        }
    }

private:
    struct fsm{
        FILE *file_ptr; // open file

        int thr_id; // thread id


        int state;
        int calc_num;
        int mark;
    };
};

void *thr_handler(void *ptr){

    // init a struct fsm
    void *fsm_ptr;
    fsm_ptr = prime_fsm::fsm_init();


    // set pthread id
    prime_fsm::fsm_setthrid(fsm_ptr, *(int *)ptr);


    while (prime_fsm::fsm_getstate(fsm_ptr) != STATE_T){ // do while till state is not terminate
        prime_fsm::fsm_driver(fsm_ptr);
    }

    pthread_exit(ptr);
}

int main(int argc, char **argv){
    int i;
    void *ptr;
    pthread_t tid[pthread_max];

    // create 5 pthread
    for (i = 0; i < pthread_max; i++){
        int *arg = new int(i);
        pthread_create(tid+i, nullptr, &thr_handler, arg);
    }

    // recycle pthread and delete arg
    for (i = 0; i < pthread_max; i++){
        pthread_join(tid[i], &ptr);
        delete (int *)ptr;
    }

    // destory the global mutex and condition
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);
    exit(0);
}

