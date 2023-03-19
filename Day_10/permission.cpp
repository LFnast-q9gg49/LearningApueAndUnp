#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

// this file is to test real permission and effective permission

int main(int argc, char **argv){
	pid_t pid = 0;
	uid_t ruid = 0, euid = 0;

	ruid = getuid();
	euid = geteuid();

	fprintf(stdout, "user real permission is %d and effective permission is %d\n", ruid, euid);
	fflush(nullptr);

	pid = fork();
	if (pid < 0){
		fprintf(stderr, "fork error : %s\n", strerror(errno));
		exit(1);
	}else if (pid == 0){
		// child
		execl("./change", "change", nullptr);
		perror("exec error");
		exit(1);
	}else{
		sleep(100);
		// parent
		wait(nullptr);
	}
	
	exit(0);
}
