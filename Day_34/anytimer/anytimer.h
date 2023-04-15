#ifndef ANYTIMER_H
#define ANYTIMER_H

const int job_max = 1024;

typedef void at_jobfunc_t(void *);

class anytimer {
public:
    static int at_addjob(int sec, at_jobfunc_t *jobp, void *arg);
    /*
     * return >= 0 success;
     *        == -EINVAL
     *        == -ENOSPC
     *        == -ENOMEM
     * */
    static int at_canceljob(int id);
    /*
     * return == 0 success;
     *        == -EINVAL;
     * */
    static int at_waitjob(int id);
    /*
     * return == 0 success;
     *        == -EINVAL;
     * */
    static int at_repeatjob(int sec, at_jobfunc_t *jobp, void *arg);
    /*
     * return >= 0 success;
     *        == -EINVAL
     *        == -EBUSY // job is a cycle job
     *        == -ECANCELED // job is canceled
     * */
};


#endif

