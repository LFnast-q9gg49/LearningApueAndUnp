#include "mytbf.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef void (*sighandler_t)(int);

static sighandler_t alrm_handler_save;
static struct mytbf_st *job[MYTBF_MAX];
static int inited = 0;

struct mytbf_st{
		int cps;
		int burst;
		int token;
		int pos;
};

static void alrm_handler(int s){
		alarm(1);
		for(auto i = 0; i < MYTBF_MAX; i++){
				if (job[i] != nullptr){

						job[i]->token += job[i]->cps;
						if (job[i]->token > job[i]->burst){
							job[i]->token = job[i]->burst;
						}
				}
		}
}

static void module_unload(void){
		int i;
		signal(SIGALRM, alrm_handler_save);
		alarm(0);
		for (auto j = 0; j < MYTBF_MAX; j++){
				free(job[j]);
		}
}

static void module_load(void){
		alrm_handler_save = signal(SIGALRM, alrm_handler);
		alarm(1);

		atexit(module_unload);
}

static int get_free_pos(void){
		for (auto i = 0; i < MYTBF_MAX; i++){
				if(job[i] == nullptr){
					return i;
				}
		}
		return -1;
}

mytbf_t *mytbf_init(int cps, int burst){
		struct mytbf_st *me;
		
		if (!inited){
				module_load();
				inited = 1;
		}

		auto pos = get_free_pos();
		if (pos < 0){
				return nullptr;
		}
		me = malloc(sizeof(*me));
		if (me == nullptr){
				return nullptr;
		}
		me->token = 0; 
		me->cps = cps;
		me->burst = burst;
		me->pos = pos;
		job[pos] = me;
		
		return me;
}

static int min(int a, int b){
		if (a < b){
			return a;
		}
		return b;
}

int mytbf_fetchtoken(mytbf_st *ptr, int size){
		struct mytbf_st *me = ptr;
		if(size <= 0){
				return -1;
		}
		while (me->token <= 0){
				pause();
		}
		auto n = min(me->token, size);

		me->token -= n;
		return n;
}

int mytbf_returntoken(mytbf_st *ptr, int size){
		struct mytbf_st *me = ptr;
		if (size <= 0){
				return -1;
		}
		me->token += size;
		if(me->token > me->burst){
				me->token = me->burst;
		}
		return size;
}

int mytbf_destory(mytbf_st *ptr){
		struct mytbf_st *me = ptr;
		job[me->pos] = nullptr;
		free(ptr);
		return 0;
}

