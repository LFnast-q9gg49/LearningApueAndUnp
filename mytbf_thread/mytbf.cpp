#include "mytbf.h"

static struct mytbf_st *job[MYTBF_MAX]; // mutex

static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t once_control = PTHREAD_ONCE_INIT;

static pthread_t tid_alrm;

struct mytbf_st{
    int cps;
    int burst;
    int token;
    int pos;
    pthread_mutex_t mut;
    pthread_cond_t cond;
};

static int get_free_pos_unlock(){
    for (auto i = 0; i < MYTBF_MAX; i++){
        if (job[i] == nullptr){
            return i;
        }
    }
    return -1;
}

static int min(int a, int b){
    return a<b?a:b;
}

static void module_unload(){
    /*
    sigaction(SIGALRM, &alrm_save, nullptr);

    // more accurate alarm(0)
    struct itimerval itv{};
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, nullptr);// stop alrm
    */

    pthread_cancel(tid_alrm);
    pthread_join(tid_alrm, nullptr);

    for (auto & i : job){
        if (i != nullptr) {
            mytbf_destroy(i);
        }
    }

    pthread_mutex_destroy(&mut_job);
}

static void module_load(){
    /*
    auto alrm_action = [](int s, siginfo_t *infoptr, void *unused)->void{

        // more accurate alarm(1)
        struct itimerval itv{};
        itv.it_interval.tv_sec = 1;
        itv.it_interval.tv_usec = 0;
        itv.it_value.tv_sec = 1;
        itv.it_value.tv_usec = 0;
        setitimer(ITIMER_REAL, &itv, nullptr);

        for(auto & i : job){
            if(i != nullptr){
                i->token += i->cps;
                if (i->token > i->burst){
                    i->token = i->burst;
                }
            }
        }
    };

    struct sigaction sa{};
    sa.sa_sigaction = alrm_action;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO; // use three param function

    sigaction(SIGALRM, &sa, &alrm_save);

    // more accurate alarm(1)
    struct itimerval itv{};
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, nullptr);
    */

    auto thr_alrm = [] (void *) -> void *{

        while(true) {
            pthread_mutex_lock(&mut_job);
            for (auto &i: job) {
                if (i != nullptr) {
                    pthread_mutex_lock(&i->mut);
                    i->token += i->cps;
                    if (i->token > i->burst) {
                        i->token = i->burst;
                    }
                    pthread_cond_broadcast(&i->cond);
                    pthread_mutex_unlock(&i->mut);
                }
            }
            pthread_mutex_unlock(&mut_job);
            sleep(1);
        }
    };

    int err;

    err = pthread_create(&tid_alrm, nullptr, thr_alrm, nullptr);
    if (err){
        fprintf(stderr, "error occured : %s\n", strerror(err));
        exit(1);
    }
    atexit(module_unload);
}

mytbf_t *mytbf_init(int cps, int burst){
    int pos;
    auto *me = new mytbf_st;

    /*
    pthread_mutex_lock(&mut_job);
    if (!inited){
        module_load();
        inited = 1;
    }
    pthread_mutex_unlock(&mut_job);
    */

    pthread_once(&once_control, module_load);

    me -> token 	= 0;
    me -> cps 	    = cps;
    me -> burst 	= burst;
    me -> pos 	    = pos;
    pthread_mutex_init(&me->mut, nullptr);
    pthread_cond_init(&me->cond, nullptr);

    pthread_mutex_lock(&mut_job);
    pos = get_free_pos_unlock();
    if (pos < 0){
        pthread_mutex_unlock(&mut_job);
        delete me;
        return nullptr;
    }

    job[me->pos] = me;

    pthread_mutex_unlock(&mut_job);
    return me;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size){
    int tmp;
    auto *me = (struct mytbf_st *)ptr;

    if (size <= 0){
        return -EINVAL;
    }

    pthread_mutex_lock(&me->mut);
    while (me -> token <= 0){
        pthread_cond_wait(&me->cond, &me->mut); // unlock and wait for signal or broadcast
//        pthread_mutex_unlock(&me->mut);
//        sched_yield();
//        pthread_mutex_lock(&me->mut);
    }

    tmp = min(me -> token, size);
    me->token -= tmp;
    pthread_mutex_unlock(&me->mut);
    return tmp;
}

int mytbf_returntoken(mytbf_t *ptr, int size){

    auto *me = (struct mytbf_st *)ptr;

    if (size <= 0){
        return -EINVAL;
    }

    pthread_mutex_lock(&me->mut);

    me->token += size;
    if (me->token > me->burst){
        me->token = me->burst;
    }
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);

    return size;
}

int mytbf_destroy(mytbf_t *ptr){
    auto *me = (struct mytbf_st *)ptr;

    pthread_mutex_lock(&mut_job);
    job[me->pos] = nullptr;
    pthread_mutex_unlock(&mut_job);
    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(ptr);

    return 0;
}
