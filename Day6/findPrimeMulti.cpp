#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

const int start = 30000000;
const int end   = 30000200;

int main(){
		int mark;
		pid_t pid;
		for (auto i = start; i <= end; i++){
			pid = fork();
			if (pid < 0){
					// error
				perror("fork failed");
				exit(1);
			}

			if (pid == 0){
				mark = 1;
				for (auto j = 2; j < i/2; j++){
					if (i%j == 0){
							mark = 0;
							break;
					}
				}
		
				if (mark){
					fprintf(stdout, "%d is a primer\n", i);
				}
				exit(0); // important !!!
			}
		}
		exit(0);
}
