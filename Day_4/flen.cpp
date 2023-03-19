#include <cstdio>
#include <cstdlib>

#include <cerrno>

#include <sys/types.h>
#include <sys/stat.h>

static off_t flen(const char* fname){
	static struct stat statres;
	if (stat(fname, &statres)<0){
		perror("error");
		exit(1);
	}

	return statres.st_size;
}

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "Usage ...\n");
		exit(1);
	}

	fprintf(stdout, "total size is %ld\n", flen(argv[1]));

	exit(0);
}
