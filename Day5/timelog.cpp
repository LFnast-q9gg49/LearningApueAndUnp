// this file is a timelog. if you run it, it will write y-m-d h:m:s into the file you pointed, 
// also with the line number in append mode
//

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string>
#include<unistd.h>

const int MAXSIZE = 128;

int main(int argc, char **argv){
		FILE *desfile = nullptr;
		int lines = 0;
		time_t curtime;
		struct tm *cur_tm;
		char msg[MAXSIZE];
		if (argc<2){
				fprintf(stdout, "Usage: ./timelog <destination file>\n");
				exit(1);
		}

		if ((desfile = fopen(argv[1], "a+")) == nullptr ){
				perror("file open error");
				exit(1);
		}
		
		// count file lines, we need to show line number
		while( fgets(msg, MAXSIZE, desfile) != nullptr){
				lines++;
		}

		while (1){
				if ((time(&curtime)) == (time_t)-1){
						perror("get time error");
						exit(1);
				}
				cur_tm = localtime(&curtime);

				fprintf(desfile, "%d %d-%d-%d %d:%d:%d\n", ++lines, cur_tm->tm_year + 1900, 
								cur_tm->tm_mon + 1,
								cur_tm->tm_mday, cur_tm->tm_hour, 
								cur_tm->tm_min,
								cur_tm->tm_sec);
				// we need to flush, in file '\n' will not flush buffer
				fflush(desfile);
				sleep(1);
		}
	    
		fclose(desfile);

		exit(0);
}
