#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
	FILE *fps = nullptr, *fpd = nullptr;
	int ch = 0;
	if (argc < 3){
		fprintf(stderr, "Usage: %s <src_fileName> <dst_fileName>\n", argv[0]);
		exit(1);
	}

	if ((fps = fopen(argv[1], "r")) == nullptr){
		perror("source file open error : ");
		exit(1);
	}
	if ((fpd = fopen(argv[2], "w")) == nullptr){
		perror("destination file open error : ");
		exit(1);
	}

	while(1){
		if ((ch = fgetc(fps)) == EOF){
			break;
		}
		fputc(ch, fpd);
	}
	fclose(fpd);
	fclose(fps);
	exit(0);
}
