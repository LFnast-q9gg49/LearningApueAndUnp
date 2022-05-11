#include<stdio.h>
#include<stdlib.h>

const int BUFSIZE = 1024;

int main(int argc, char **argv){
	FILE *fps = nullptr;
	
	if (argc < 2){
		fprintf(stderr, "Usage: %s <src_fileName> \n", argv[0]);
		exit(1);
	}

	if ((fps = fopen(argv[1], "r")) == nullptr){
		fclose(fps);
		perror("error happened : ");
	}

	fseeko(fps, 0, SEEK_END);
	fprintf(stdout, "%ld\n", ftello(fps));

	fclose(fps);
	exit(0);
}
