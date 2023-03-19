#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char **argv){
	int fd;
	
	if (argc < 2){
		// err
		exit(1);
	}

	fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0600);
	if (fd < 0){
		// err
		exit(1);
	}

	lseek(fd, 32767, SEEK_SET);

	write(fd, "a", 1);

	close(fd);
	
	exit(0);
}
