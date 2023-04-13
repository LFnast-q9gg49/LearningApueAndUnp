#include "anytimer.h"
#include <cstdio>
#include <cstdlib>
#include <cerrno>

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

static int get_free_pos(){
    for (auto i = 0; i < job_max; i++){
        if(job[i] == nullptr){
            return i;
        }
    }
    return -1;
}

int anytimer::at_addjob(int sec, void (*jobp)(void *), void *arg) {
    int pos;

    pos = get_free_pos();
    if (pos < 0){
        return -ENOSPC;
    }

    auto me = new (struct at_job_st);
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

    }

    return 0;
}

int anytimer::at_waitjob(int id) {
    return 0;
}

int anytimer::at_pausejob(int id) {
    return 0;
}

