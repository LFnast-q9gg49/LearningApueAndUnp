#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <unistd.h>
int main(){
	pid_t pid;
	auto info = [](void)-> void {fprintf(stdout, "my PPID is %d, PID is %d, GPID is %d, SID is %d\n", getppid(), getpid(), getpgid(0), getsid(0));};
	info();
	fflush(nullptr);

	pid = fork();
	if (pid < 0){
		fprintf(stderr, "fork error : %s\n", strerror(errno));
		exit(0);
	}else if (pid == 0){
		info();
		fflush(nullptr);
		setsid();
		info();
		while(1);
	}

	exit(0);
}
