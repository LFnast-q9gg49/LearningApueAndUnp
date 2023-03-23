#ifndef MYSEM_H
#define MYSEM_H

typedef void mysem_t;

class mysem {
public:
    static mysem_t *mysem_init(int init_val);

    static int mysem_add(mysem_t *, int);
    static int mysem_sub(mysem_t *, int); // only for remain > demand

    static int mysem_destory(mysem_t *);
};


#endif

