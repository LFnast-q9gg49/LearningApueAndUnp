#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
const int SIZE = 100;
int main(int argc, char **argv){
	int src_fd = 0, dst_fd = 0;
	char buf[SIZE];
	int cnt = 0;

	if (argc < 3){
		fprintf(stderr, "Usage : ./mycp_2 <src_file> <dst_file>\n");
		exit(1);
	}
	if ((src_fd = open(argv[1], O_RDONLY)) < 0){
fprintf(stderr, "Source File Open Failed : %s\n", strerror(errno));
		exit(1);
	}
	if ((dst_fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666)) < 0){
		close(src_fd);
		fprintf(stderr, "Destination File Open Failed : %s\n", strerror(errno));
		exit(1);
	}

	while((cnt = read(src_fd, buf, SIZE)) > 0){
		if (write(dst_fd, buf, cnt) < 0){
			break;
		}
	}

	close(dst_fd);
	close(src_fd);	
	exit(0);
}
