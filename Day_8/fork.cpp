#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <unistd.h>

int main(){
	pid_t pid;

	fprintf(stdout, "[%d] Begin\n", getpid());

	fflush();

	id = fork();
	if (pid < 0){
		perror("fork error");
		exit(1);
	}if (pid == 0){
		// child
		//
		fprintf(stdout, "I'm child and pid is %d\n", getpid());
	}else{
		// parent
		//
		fprintf(stdout, "I'm parent and pid is %d\n", getpid());

	}

        fprintf(stdout, "[%d] Ends\n", getpid());
	getchar();
	exit(0);
}
