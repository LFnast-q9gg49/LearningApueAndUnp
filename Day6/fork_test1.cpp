#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
		pid_t pid;
		fprintf(stdout, "[pid:%d] Begin.\n", getpid());
		// terminal buffer is different from file
		// you need this !!!
		fflush(nullptr);

		pid = fork();
		if (pid < 0){
				perror("child process fork failed");
				exit(1);
		}else if (pid == 0){
				fprintf(stdout, "[pid:%d] Child Running.\n", getpid());
		}else{
				fprintf(stdout, "[pid:%d] Parent Running.\n", getpid());
		}
		fprintf(stdout, "[pid:%d] End.\n", getpid());
		exit(0);
}
