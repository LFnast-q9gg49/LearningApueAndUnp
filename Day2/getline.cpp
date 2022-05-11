#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
	FILE *fp = nullptr;
	char *lineBuffer = nullptr;
	size_t lineSize = 0;
	if (argc < 2){
		fprintf(stderr, "no file\n");
		exit(1);
	}

	if ((fp = fopen(argv[1], "r")) == nullptr){
		fclose(fp);
		perror("open file error : ");
		exit(1);
	}

	while(1){
		if ((getline(&lineBuffer, &lineSize, fp)) < 0){
			break;
		}
		fprintf(stdout, "%ld\n", strlen(lineBuffer));
		fprintf(stdout, "%ld\n", lineSize);
	}
	exit(0);
}
