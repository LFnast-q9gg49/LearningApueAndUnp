#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define FNAME "./outout"

int main(){
	int fd;

	fd = open(FNAME, O_WRONLY|O_CREAT|O_TRUNC, 0600);
	if(fd < 0) {
		perror("open");
		exit(1);
	}

	/*
	close(1);
	dup(fd);
	*/
	// thus we need atom operation
	
	dup2(fd, 1);
	if (fd != 1){
		close(fd);
	}

	puts("hello !");

	exit(0);
}
