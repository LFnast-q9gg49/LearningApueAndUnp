#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

static int ftype(const char *fname){
	struct stat statres;
	if (stat(fname, &statres) < 0){
		perror("stat error");
		exit(1);
	}

	// we just judge three types of file
	if (S_ISREG(statres.st_mode)){
		return '-';
	}else if (S_ISDIR(statres.st_mode)){
		return 'd';
	}else if (S_ISSOCK(statres.st_mode)){
		return 's';
	}else {
		return '?';
	}
}

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stdout, "format error: ./ftype <sourcefile>");
		exit(1);
	}

	fprintf(stdout, "%c\n", ftype(argv[1]));

	exit(0);
}


