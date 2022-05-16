#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int main(){
		pid_t pid;
		fprintf(stdout, "Start\n");
		fflush(nullptr);
		pid = fork();
		if (pid < 0){
				perror("fork perror");
				exit(1);
		}else if(pid == 0){
				// child
				execl("/bin/sleep", "guess", "100", nullptr);
				perror("execl error");
				exit(1);
		}
		wait(nullptr);
		fprintf(stdout, "End\n");
		exit(0);
}
