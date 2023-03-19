#include "mytbf.h"

typedef void (*sighandler_t)(int);

static sighandler_t save;
static struct sigaction alrm_save;

static struct mytbf_st *job[MYTBF_MAX];

static int inited = 0;

struct mytbf_st{
    int cps;
    int burst;
    int token;
    int pos;
};

static int get_free_pos(){
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
    sigaction(SIGALRM, &alrm_save, nullptr);

    // more accurate alarm(0)
    struct itimerval itv;
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, nullptr);// stop alrm

    for (auto & i : job){
        free(i);
    }
}

static void module_load(){
    auto alrm_action = [](int s, siginfo_t *infoptr, void *unused)->void{
	
        // more accurate alarm(1)
        struct itimerval itv;
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

    struct sigaction sa;
    sa.sa_sigaction = alrm_action;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO; // use three param function

    sigaction(SIGALRM, &sa, &alrm_save);

    // more accurate alarm(1)
    struct itimerval itv;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, nullptr);

    atexit(module_unload);
}

mytbf_t *mytbf_init(int cps, int burst){
    int pos;
    auto *me = (mytbf_st *)malloc(sizeof(mytbf_st));

    if (inited == 0){
        module_load();
        inited = 1;
    }
    pos = get_free_pos();
    if (pos < 0){
        return nullptr;
    }
    if (me == nullptr){
        return nullptr;
    }
    me -> token 	= 0;
    me -> cps 	    = cps;
    me -> burst 	= burst;
    me -> pos 	    = pos;

    job[me->pos] = me;
    return me;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size){
    int tmp;
    auto *me = (mytbf_st *)ptr;

    if (size <= 0){
        return -EINVAL;
    }

    while (me -> token <= 0){
        pause();
    }

    tmp = min(me -> token, size);
    me->token -= tmp;
    return tmp;
}

int mytbf_returntoken(mytbf_t *ptr, int size){

    auto *me = (mytbf_st *)ptr;

    if (size <= 0){
        return -EINVAL;
    }

    me->token += size;
    if (me->token > me->burst){
        me->token = me->burst;
    }

    return size;
}

int mytbf_destroy(mytbf_t *ptr){
    auto *me = (mytbf_st *)ptr;
    job[me->pos] = nullptr;
    free(ptr);

    return 0;
}

