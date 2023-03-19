#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main(){
	int i = 0, rt = 0;

	for(; i < 20; i++){
		rt = fork();
		if (rt < 0){
			fprintf(stderr, "fork error : %s\n", strerror(errno));
		}else if (rt > 0){
			// parent
			//
			
		}else {
			// sleep(100);
			exit(0);
		}
	}
	// sleep(100);
	//
	i ^= i;
	for (; i < 20; i++){
		wait(nullptr);
	}
	exit(0);
}
