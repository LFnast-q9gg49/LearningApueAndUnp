#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
// this file is to use —————— date +%s

int main(){
		fprintf(stdout, "Begin\n");
		fflush(nullptr);
		execl("/bin/date", "date" ,"+%s", nullptr);
		// if success, it will never return
		perror("execl()");
		exit(1);

		fprintf(stdout, "End\n");

		exit(0);
}
