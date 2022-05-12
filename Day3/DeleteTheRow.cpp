// this is a test for deleting the 10th row of a file.
// using sysio to complete the operation
// we assumed that this file has more than 10 rows
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>

const int MAXSIZE = 1;

int main(int argc, char **argv){
	int fd = 0, offset1 = 0, offset2 = 0, cnt = 0;
	char buf[MAXSIZE];

	if (argc < 2){
		fprintf(stdout, "usage error: ./DeleteTheRow <fileName>");
		exit(1);
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0){
		perror("file open error");
		exit(1);
	}

	// count char
	//
	while(read(fd, buf, MAXSIZE) > 0){
		if(buf[0] == '\n'){
			cnt++;
		}
		if (cnt <= 9){
			offset1++;
		}
		if (cnt <= 10){
			offset2++;
		}
	}	
	cnt = 0;
	lseek(fd, offset2, SEEK_SET);
	while(read(fd, buf, MAXSIZE) > 0){
		cnt++;
		lseek(fd, offset1 - offset2, SEEK_CUR);
		write(fd, buf, MAXSIZE);
		lseek(fd, offset2+cnt, SEEK_SET);
	}

	close(fd);
	exit(0);
}
