#include <cstdlib>
#include <cerrno>
#include <csignal>
#include <unistd.h>
#include "mytbf.h"

typedef void (*sighandler_t)(int);

static sighandler_t save;

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
    signal(SIGALRM, save);
    alarm(0);
    for (auto & i : job){
        free(i);
    }
}

static void module_load(){
    auto alrm_handler = [](int)->void{
        alarm(1);
        for(auto & i : job){
            if(i != nullptr){
                i->token += i->cps;
                if (i->token > i->burst){
                    i->token = i->burst;
                }
            }
        }
    };
    save = signal(SIGALRM, alrm_handler);
    alarm(1);

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

