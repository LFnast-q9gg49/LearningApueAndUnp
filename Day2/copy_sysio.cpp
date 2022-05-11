#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
const int MAXSIZE = 1024;

int main(int argc, char **argv){
	int fds = 0, fdd = 0, len = 0;
	char buf[MAXSIZE];

	if (argc < 3){
		fprintf(stderr, "format error : ./copy <source file> <destination file>");
		exit(1);
	}

	if ((fds = open(argv[1], O_RDONLY)) < 0){
		close(fds);
		perror("source file open error");
		exit(1);
	}

	if ((fdd = open(argv[2], O_WRONLY|O_CREAT, O_TRUNC, 0666)) < 0){
		close(fdd);
		perror("destination file open error");
		exit(1);
	}

	while(1){
		len = read(fds, buf, MAXSIZE);
		if (len < 0){
			perror("read error");
			break;
		}else if (len == 0){
			break;
		}

		if (write(fdd, buf, len) < 0){
			perror("write error");
			break;
		}
	}

	close(fdd);
	close(fds);

	exit(0);
}
