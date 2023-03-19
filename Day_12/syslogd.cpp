#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>
int main(){
	pid_t pid;
	auto info = [](void)-> void {fprintf(stdout, "my PPID is %d, PID is %d, GPID is %d, SID is %d\n", getppid(), getpid(), getpgid(0), getsid(0));};
	info();
	
	openlog("mydaemon", LOG_PID, LOG_DAEMON);
	
	fflush(nullptr);
	
	pid = fork();
	if (pid < 0){
		fprintf(stderr, "fork error : %s\n", strerror(errno));
		exit(0);
	}else if (pid == 0){
		info();
		fflush(nullptr);	
		syslog(LOG_INFO, "daemon() successded");
                closelog();

		setsid();
		info();

		while(1);
	}
	closelog();
	exit(0);
}
