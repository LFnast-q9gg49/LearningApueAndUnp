#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

const char *pattern = "/etc/*";

#if 0
static int errfunc_(const char *errpath, int errno){
	puts(errpath);
	fprintf(stderr, "error message : %s\n", strerror(errno));
	return 0;
}

#endif

int main(int argc, char **argv){
	glob_t globres;
	int err;

	err = glob(pattern, 0, nullptr, &globres);
	if (err){
		fprintf(stderr, "error code : %d", err);
		exit(1);
	}

	for (auto i = 0; i < globres.gl_pathc; i++){
		fprintf(stdout, "%s\n", globres.gl_pathv[i]);
	}

	globfree(&globres);
	exit(0);
}
