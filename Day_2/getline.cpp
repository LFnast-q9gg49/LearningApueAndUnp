#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <string.h>

int main(int argc, char **argv){
	FILE *fp;
	char *linebuf = nullptr;
	size_t linesize = 0;

	if (argc < 2){
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	
	fp = fopen(argv[1], "r");
	if (fp == nullptr){
		fprintf(stderr, "error is : %s", strerror(errno));
	}
	

	while(1){
		if (getline(&linebuf, &linesize, fp) < 0){
			break;
		}
		fprintf(stdout, "%ld\n", strlen(linebuf));
		fprintf(stdout, "%ld\n", linesize);
	}

	exit(0);
}

