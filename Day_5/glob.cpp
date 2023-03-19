#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <glob.h>

const char *path = "/etc/.*";

int errfunc(const char *path, int eerrno){
	fprintf(stderr, "error occured : %s\n", strerror(errno));
	return 0;
}

int main(){
	glob_t pglob;
	int cnt = 0;

	glob(path, 0, &errfunc, &pglob);
	
	
	for (cnt; cnt < pglob.gl_pathc; cnt++){
		fprintf(stdout, "%s\n", pglob.gl_pathv[cnt]);
	}

	globfree(&pglob);
	exit(0);
}
