#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *func(void *){
	int i;
	fprintf(stdout, "addr is %d\n", &i);
	while(1){
		pause();
	}
	pthread_exit(nullptr);
}

int main(){
	pthread_t tid;
	int i;
	int err;

	pthread_attr_t attr;

	pthread_attr_init(&attr);
	
	pthread_attr_setstacksize(&attr, 1<<24);

	for (i = 0; ; i++){
		err = pthread_create(&tid, &attr, &func, nullptr);
		if (err){
			fprintf(stderr, "pthread_create() : %s\n", strerror(err));
			break;
		}
	}
	
	pthread_attr_destroy(&attr);

	fprintf(stdout, "max thread is %d\n", i);
	exit(0);
}
