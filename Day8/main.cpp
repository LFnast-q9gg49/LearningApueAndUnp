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
#include "mytbf.h"

const int CPS = 10;
const int BFS = 1024;
const int BURST = 100;
static volatile int token = 0; 

int main(int argc, char **argv){
		int sfd = 0;
		int dfd = 1;
		char buf[BFS];
		int len, ret, pos;
		mytbf_t *tbf;

		if (argc < 2){
				fprintf(stdout, "Usage error: ./\n");
				exit(1);
		}
		tbf = mytbf_init(CPS, BURST);
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
				size = mytbf_fecthtoken(tbf, BFS);

				while((len = read(sfd, buf, size)) < 0){
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
				if(size - len > 0){
						mytbf_returntoken(tbf, size-len);
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
		mytbf_destory(tbf);
		exit(0);
}
