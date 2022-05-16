#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<syslog.h>
#include<string.h>

const char *FILE_NAME = "/home/out";

static int daemonize(void){
		pid_t pid;
		int fd;
		pid = fork();
		if (pid < 0){
				return -1;
		}
		if (pid > 0){
				exit(0);
		}
		fd = open("/dev/null", O_RDWR);
		if (fd < 0){
				return -1;
		}
		dup2(fd, 0);
		dup2(fd, 1);
		dup2(fd, 2);
		if (fd > 2){
				close(fd);
		}
		setsid();
		chdir("/");
		umask(0);
		return 0;
}

int main(){
		FILE *fp = nullptr;

		openlog("daemon_test", LOG_PID, LOG_DAEMON);
		if (daemonize()){
				syslog(LOG_ERR, "daemonize failed");
				exit(1);
		}else{
				syslog(LOG_INFO, "daemonize success");
		}
		fp = fopen(FILE_NAME, "w");
		if (fp == nullptr){
				syslog(LOG_ERR,"open error:%s", strerror(errno));
				exit(1);
		}
		syslog(LOG_INFO, "%s was opened.", FILE_NAME);

		for (auto i = 0; ; i++){
				fprintf(fp, "%d\n", i);
				syslog(LOG_INFO, "%d is registered in the file.", i);
				fflush(fp);
				sleep(1);
		}

		exit(0);
}
