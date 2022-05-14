#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
 
/* you can type ./commandTime -x to get different infos
 * -y year -m month -d day -H hour -M minute -S second
 * */
 
const int MAXSIZE = 128;
const int FMTSIZE = 128;
 
int main(int argc, char **argv){
         FILE *desfile = stdout;
         int c = 0;
         time_t curtime;
         struct tm *cur_tm;
         char msg[MAXSIZE];
         char fmtstr[FMTSIZE];
         fmtstr[0] = '\0';

 
         if (time(&curtime) == (time_t)-1){
                 perror("time error");
                 exit(1);
         }
 		 cur_tm = localtime(&curtime);
         while(1){
             c = getopt(argc, argv, "-H:MSy:md");
             if (c<0){
                     break;
             }
             switch(c){
			 case 1:
					 if (desfile == stdout){
					 	desfile = fopen(argv[optind - 1], "w");
					 	if (desfile == nullptr){
						 	perror("file open error");
							 desfile = stdout;
							 break;
					 	}
					 }
					 break;
             case 'H':
					 if (strcmp(optarg, "12") == 0){
							 strncat(fmtstr, "%I(%P)", FMTSIZE);
					 }else if (strcmp(optarg, "24") == 0){
							 strncat(fmtstr, "%H", FMTSIZE);
					 }else {
							 fprintf(stderr, "-H error usage");
					 }
                     break;
             case 'M':
                     strncat(fmtstr, "%M", FMTSIZE);
                     break;
             case 'S':
					 strncat(fmtstr, "%S", FMTSIZE);
                     break;
             case 'y':
					 strncat(fmtstr, "%y", FMTSIZE);
                     break;
             case 'm':
					 strncat(fmtstr, "%m", FMTSIZE);
                     break;
             case 'd':
					 strncat(fmtstr, "%d", FMTSIZE);
                     break;
             default:
                     break;
             }       
         }   

		 strncat(fmtstr, "\n", FMTSIZE);
         strftime(msg, MAXSIZE, fmtstr, cur_tm);
         puts(msg);

		 if (desfile != stdout){
				 fclose(desfile);
		 }
         exit(0);
}

