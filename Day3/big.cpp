#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	int fd = -1;

	if (argc < 2){
		fprintf(stdout,"format error: ./big <filename>" );
		exit(1);
	}

	fd = open(argv[1], O_CREAT|O_WRONLY|O_TRUNC, 0600);
	if (fd < 0){
		perror("open error");
		exit(1);
	}

	lseek(fd, 5LL*1024LL*1024LL*1024LL-1LL, SEEK_SET);
	write(fd,"",1);
	close(fd);
	exit(0);
}
