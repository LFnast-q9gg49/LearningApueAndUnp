// I/O

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(){
	FILE *fp;

	fp = fopen("tmp", "r+dsufjaguu[pdsbabsdgppudsa");
	if (fp == nullptr){
		fprintf(stderr, "file open failed , errno is : %d\n", errno);
		exit(1);
	}
	puts("ok!");
	exit(0);
}
