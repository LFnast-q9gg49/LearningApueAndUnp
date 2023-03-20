#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

static void * function(void *ptr){
	fprintf(stdout, "thread is working\n");
	pthread_exit(nullptr);
}

int main(){
	pthread_t tid;
	int en;

	fprintf(stdout, "Begin\n");

	en = pthread_create(&tid, nullptr, &function, nullptr);
	if (en != 0){
		fprintf(stderr, "pthread_create error occured %s\n", strerror(en));
		exit(0);
	}

	// pthread_join(tid, nullptr);
	
	fprintf(stdout, "End\n");

	exit(0);
}
