#include <cstdio>
#include <cstdlib>

#include <unistd.h>

const int LEFT = 30000000;
const int RIGHT = 30000200;

int main(){
	int i, j ,mark;
	pid_t pid;

	for (i = LEFT; i <= RIGHT; i++){
		mark = 1;

		pid = fork();

		if (pid < 0){
			// err
			exit(1);
		}else if(pid == 0){
			for (j = 2; j < i/2; j++){
                        	if (i % j == 0){
                                	mark = 0;
                                	break;
                        	}
                	}
                	if (mark){
                        	fprintf(stdout, "%d is a primer\n", i);
                	}
			sleep(100);
			exit(0);
		}
		
		
	}
	exit(0);
}
