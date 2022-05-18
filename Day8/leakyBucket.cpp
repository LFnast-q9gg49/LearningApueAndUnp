// slow cat
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

const int CPS = 10;
const int BFS = CPS;
static volatile int loop = 0; 

static void alrm_handler(int s){
		alarm(1);
		loop = 1;
} 

int main(int argc, char **argv){
		int sfd = 0;
		int dfd = 1;
		char buf[BFS];
		int len, ret, pos;

		if (argc < 2){
				fprintf(stdout, "Usage error: ./\n");
				exit(1);
		}

		signal(SIGALRM, alrm_handler);
		alarm(1);
		do{
				sfd = open(argv[1], O_RDONLY);
				if (sfd < 0){
						if(errno != EINTR){
								perror("open error");
								exit(1);
						}

				}
		}while(sfd < 0);

		while(1){
				while(!loop){
						pause();
				}

				loop = 0;

				while((len = read(sfd, buf, BFS)) < 0){
					if (len < 0){
						if (errno != EINTR){
								continue;
						}
						perror("read\n");
						break;
					}
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
								perror("write");
								exit(1);
						}
						pos += ret;
						len -= ret;
				}
		}
		close(sfd);
		exit(0);
}
