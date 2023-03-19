#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <fcntl.h>
#include <signal.h>
static volatile int loop = 0;

const int SIZE = 10;

int main(int argc, char **argv){
	int sfd = -1, dfd = 1;
	char buf[SIZE];
	int len = 0, ret = 0, pos = 0;

	// lambda
	auto error_handler = []()->void{
		fprintf(stderr, "error occured %s\n", strerror(errno));
		exit(1);
	};
	auto alrm_handler = [](int)->void{
		alarm(1);
		loop = 1;
	};

	
	signal(SIGALRM, alrm_handler);
	alarm(1);

	if (argc < 2){
		error_handler();
	}
	do{
		sfd = open(argv[1], O_RDONLY);
		if (sfd < 0){
			if (errno != EINTR){
				error_handler();
			}
		}
	}while(sfd < 0);

	while(1){
		while(!loop){
			pause();
		}
		loop = 0;

		while(len = read(sfd, buf, SIZE) < 0){
			if (errno == EINTR){
				continue;
			}
			perror("read()");
			break;
		}
		if (len == 0){
			break;
		}
		pos = 0;
		while(len > 0){
			ret = write(dfd, buf+pos, len);
			if (ret < 0){
				if (errno == EINTR){
					continue;
				}
				error_handler();
			}
			pos += ret;
			len -= ret;
		}
	}
	close(sfd);

	exit(0);
}
