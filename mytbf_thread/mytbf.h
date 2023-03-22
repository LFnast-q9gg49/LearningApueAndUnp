#ifndef MYTBF_H_

#define MYTBF_MAX	1024

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

typedef void mytbf_t;

mytbf_t *mytbf_init(int cps, int burst);

int mytbf_fetchtoken(mytbf_t *, int);

int mytbf_returntoken(mytbf_t *, int);

int mytbf_destroy(mytbf_t *);



#endif

