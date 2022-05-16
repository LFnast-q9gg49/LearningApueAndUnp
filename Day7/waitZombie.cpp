#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

const int LEFT = 200;
const int RIGHT = 400;

int main(){
		pid_t pid = getpid();
		fprintf(stdout, "main start.\n");
		fflush(nullptr);
		for (auto i = LEFT; i <= RIGHT; i++ ){
				pid = fork();
				if (pid < 0){
						perror("fork error");
						exit(1);
				}else if(pid == 0){
						fprintf(stdout, "num is %d.\n", i);
						exit(0);
				}
		}
		fprintf(stdout, "main end\n");
		for (auto j = LEFT; j <= RIGHT; j++){
				wait(nullptr);
		}
		sleep(1000);
		exit(0);
}
