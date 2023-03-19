#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <time.h>
#include <unistd.h>
const int BUFSIZE = 40;

int main(int argc, char **argv){

	static FILE      *fp            = (FILE *)malloc(sizeof(FILE));
        static time_t    *rawtime       = (time_t *)malloc(sizeof(time_t));
        static struct tm *ripetime      = (struct tm *) malloc(sizeof(struct tm));
        char      	 *time_stamp    = (char *)malloc(BUFSIZE);
	
	// open file
	if (argc < 2){
		fprintf(stderr, "Usage : ./timelog <dstfile>\n");
		exit(1);
	}
	
	if ((fp = fopen(argv[1], "a+")) == nullptr){
		fprintf(stderr, "file open error : %s\n", strerror(errno));
		exit(1);
	}

	while(1){
		// get time and format
	        time(rawtime);
        	ripetime = localtime(rawtime);

        	strftime(time_stamp, BUFSIZE, "%Y-%m-%d %H-%M-%S", ripetime);

        	// output
		fwrite(time_stamp, BUFSIZE, 1, fp);
		fwrite("\n", 1, 1, fp);
		fflush(fp);
		sleep(1);
	}
	
	
	exit(0);

}
