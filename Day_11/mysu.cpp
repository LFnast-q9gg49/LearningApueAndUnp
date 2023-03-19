#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
int main(int argc, char **argv){
	pid_t pid;

	if (argc < 3){
		fprintf(stderr, "Usage ...");
		exit(1);
	}
	pid = fork();
	if (pid < 0){
		fprintf(stderr, "forkerr\n");
		exit(1);
	}else if (pid == 0){
		// child 
		//
		
		seteuid(atoi(argv[1]));
		execvp(argv[2], argv+2);
		perror("execvp");
		exit(1);
	}else {
		wait(nullptr);
	}
	exit(0);
}
