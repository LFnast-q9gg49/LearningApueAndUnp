#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(){

	FILE *fp;
	fp = fopen("tmp","r+b");
	if (fp == NULL){
		//perror("fopen()");
		fprintf(stderr, "fopen() failed, %s\n", strerror(errno));
		exit(1);
	}
	puts("OK");
	exit(0);
}
