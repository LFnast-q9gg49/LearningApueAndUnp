#include "anytimer.h"
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <signal.h>
#include <sys/time.h>

enum {
    STATE_RUNNING = 1,
    STATE_CANCELED,
    STATE_OVER,
};
struct at_job_st{
    int job_state;
    int sec;
    int time_remain;
    at_jobfunc_t *jobp;
    void *arg;
};

static struct at_job_st* job[job_max];
static int init = 0;
static struct sigaction old_sa;

static int get_free_pos(){
    for (auto i = 0; i < job_max; i++){
        if(job[i] == nullptr){
            return i;
        }
    }
    return -1;
}

static void module_unload(){
    struct itimerval timer{};
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, nullptr);

    sigaction(SIGALRM, &old_sa, nullptr);
}


static void module_load(){
    struct sigaction sa{};
    struct itimerval timer{};

    sa.sa_sigaction = [](int sig, siginfo_t *info, void *context) -> void{
//        if (info->si_code != SI_KERNEL){
//            return;
//        }
        for (auto & i : job){
            if (i != nullptr){
                if (i->job_state == STATE_RUNNING){
                    i->time_remain--;
                    if (i->time_remain <= 0){
                        i->jobp(i->arg);
                        i->job_state = STATE_OVER;
                    }
                }
            }
        }
    };
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO; // three parameters

    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    if (sigaction(SIGALRM, &sa, &old_sa) < 0){
        perror("sigaction()");
        exit(1);
    }


    if (setitimer(ITIMER_REAL, &timer, nullptr) < 0){
        perror("setitimer()");
        exit(1);
    }

    atexit(module_unload);
}

int anytimer::at_addjob(int sec, void (*jobp)(void *), void *arg) {
    int pos;

    pos = get_free_pos();
    if (pos < 0){
        return -ENOSPC;
    }

    auto me = new (struct at_job_st);

    if (!init){
        init = 1;
        module_load();
    }

    me->job_state = STATE_RUNNING;
    me->sec = sec;
    me->time_remain = me->sec;
    me->jobp = jobp;
    me->arg = arg;

    job[pos] = me;

    return pos;
}

int anytimer::at_canceljob(int id) {
    if (id < 0 || id > job_max|| job[id] == nullptr){
        return -EINVAL;
    }
    if (job[id]->job_state == STATE_CANCELED ){
        return -ECANCELED;
    }
    if (job[id]->job_state == STATE_OVER){
        return -EBUSY;
    }
    job[id]->job_state = STATE_CANCELED;

    return 0;
}

int anytimer::at_waitjob(int id) {
    if (id < 0 || id > job_max || job[id] == nullptr){
        return -EINVAL;
    }
    while (job[id]->job_state == STATE_RUNNING){
        pause();
    }

    if (job[id]->job_state == STATE_CANCELED || job[id]->job_state == STATE_OVER){
        delete job[id];
        job[id] = nullptr;
    }

    return 0;
}

int anytimer::at_pausejob(int id) {
    return 0;
}

