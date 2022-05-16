#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
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
						// sleep(1000);
						exit(0);
				}
		}
		fprintf(stdout, "main end\n");
		sleep(1000);
		exit(0);
}
