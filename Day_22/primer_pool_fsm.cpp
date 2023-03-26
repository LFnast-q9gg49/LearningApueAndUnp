// use 5 pthreads to calculate primes below 1000000
// and write them to the file
// just an exercise of FSM

#include <cstdio>
#include <cstdlib>
#include <pthread.h>

static const char       *path       = "./fsm_prime";
static const int        pthread_max = 5;
static pthread_mutex_t  mut         = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t   cond        = PTHREAD_COND_INITIALIZER;

const int LEFT = 2;
const int RIGHT = 1000000;

static int task = 0;

enum {
    STATE_O = 1, // open file
    STATE_G, // get num
    STATE_C, // calculate;
    STATE_W, // write prime to file;
    STATE_E, // exit
    STATE_T, // terminate;
};

class prime_fsm{
public:
    static void *fsm_init(){
        auto me = new struct fsm();
        me->state = STATE_O;
        return me;
    };

    static int fsm_getstate(void *ptr){
        auto *me = (struct fsm *)ptr;
        return me->state;
    };

    static void fsm_setthrid(void *ptr, int id){
        auto *me = (struct fsm *)ptr;
        me->thr_id = id;
    };

    static int fsm_driver(void *ptr){
        auto *me = (struct fsm *)ptr;
        int j;
        me->mark = 1;
        switch (me->state) {
            case STATE_O:
                me->file_ptr = fopen(path, "a");

                if (me->file_ptr == nullptr){
                    me->state = STATE_E;
                    task = -1;

                    break;
                }

                me->state = STATE_G;

                break;

            case STATE_G:

                // get number from pool
                pthread_mutex_lock(&mut);
                while (task == 0){
                    pthread_cond_wait(&cond, &mut);
                }
                if (task == -1){
                    me->state = STATE_E;
                    pthread_mutex_unlock(&mut);
                    break;
                }
                me->calc_num = task;
                task = 0;
                pthread_mutex_unlock(&mut);
                pthread_cond_broadcast(&cond);

                // start calculate
                me->state = STATE_C;

                break;

            case STATE_C:

                // better use miller-rabin prime, here I just use the most simple way
                for (j = 2; j <= me->calc_num / 2; j++){
                    if (me->calc_num % j == 0) {
                        me->mark = 0;
                        break;
                    }
                }

                if (me->mark){

                    // prime get
                    me->mark = 1;
                    me->state = STATE_W;
                } else{

                    // this num is not a prime
                    me->mark = 1;
                    me->state = STATE_G;
                }

                break;
            case STATE_W:

                fprintf(me->file_ptr, "thread [%d] calc %d is a prime\n", me->thr_id, me->calc_num);
                fflush(nullptr);
                me->state = STATE_G;

                break;
            case STATE_E:

                fclose(me->file_ptr);
                me->state = STATE_T;

                break;
            case STATE_T:
                break;
            default:
                break;
        }
        return 0;
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

    // put the num to the pool
    for (i = LEFT; i<= RIGHT; i++){
        pthread_mutex_lock(&mut);
        while (task != 0){ // wait threads fetch the task
            pthread_cond_wait(&cond, &mut);
        }
        task = i; // push num to task
        /* fprintf(stdout, "%d calc start\n", task);*/
        pthread_mutex_unlock(&mut);
        pthread_cond_broadcast(&cond); // awake all threads
    }

    // broadcast to all threads calculate is over
    pthread_mutex_lock(&mut);
    while (task != 0){ // wait threads fetch the task
        pthread_cond_wait(&cond, &mut);
    }
    task = -1; // push num to task
    pthread_mutex_unlock(&mut);
    pthread_cond_broadcast(&cond); // awake all threads

    /*fprintf(stdout, "calc end\n");*/

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

