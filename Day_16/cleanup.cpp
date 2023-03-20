#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <string.h>

void clean_func(void *p){
	puts((const char *)p);
}

void *function(void *){
	puts("thread is working");
	
	pthread_cleanup_push(clean_func, (void *)"cleanup:1");
	pthread_cleanup_push(clean_func, (void *)"cleanup:2");
 	pthread_cleanup_push(clean_func, (void *)"cleanup:3");

	puts("clean over");
	
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(0);
        pthread_cleanup_pop(1);
	
	pthread_exit(nullptr);
}

int main(){
	pthread_t tid;
	int en = 0;
	
	puts("Begin\n");
	en = pthread_create(&tid, nullptr, &function, nullptr);
	if (en != 0){
		fprintf(stderr, "create error : %s\n", strerror(en));
		exit(1);
	}

	pthread_join(tid, nullptr);
	puts("End\n");

	exit(0);
}
