#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <string.h>

int main(int argc, char **argv){
	FILE *src_fp = nullptr, *dst_fp = nullptr;
	int c = 0;

	if (argc < 3){
		fprintf(stderr, "Usage : ./mycp <src_file> <dst_file>\n");
		exit(1);
	}
	src_fp = fopen(argv[1], "r");
	if (src_fp == nullptr){
		fprintf(stderr, "Source File Open Error : %s\n", strerror(errno));
		exit(1);
	}
	dst_fp = fopen(argv[2], "w");
	if (dst_fp == nullptr){
		fclose(src_fp); // remenber close src
		fprintf(stderr, "Destination File Open Error : %s\n", strerror(errno));
		exit(1);
	}
	while((c = fgetc(src_fp)) != EOF){
		fputc(c, dst_fp);
	}
	fprintf(stdout, "Copy Finished\n");	
	fclose(dst_fp);
	fclose(src_fp);
	exit(0);
}
