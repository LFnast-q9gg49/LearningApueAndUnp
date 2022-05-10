#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv){
	FILE *fp = nullptr;
	int count = 0, ch = 0;
	if ((fp = fopen(argv[1], "r")) == nullptr){
		fclose(fp);
		perror("file open error");
		exit(1);
	}
	while((ch = fgetc(fp)) != EOF){
		count++;
	}
	fprintf(stdout, "valid char count : %d", count);
	exit(0);
}
