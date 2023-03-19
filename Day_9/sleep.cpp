#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main(){
	pid_t pid;

	fputs("Begin\n", stdout);
	fflush(nullptr);

	pid = fork();
	if (pid < 0){
		perror("fork");
		exit(1);
	}else if (pid == 0){
		execl("/bin/sleep", "httpd", "100", nullptr);
		perror("execl");
		exit(1);
	}

	wait(nullptr);
	fputs("End\n", stdout);
}
