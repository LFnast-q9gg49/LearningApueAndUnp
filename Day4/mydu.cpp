#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>
#include <string.h>

const int MAXSIZE = 128;

static int64_t MyDu(const char *path);
static bool IsPoint(char *path);

int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stdout, "Usage: ./mydu <directory>\n");
		exit(1);
	}

	fprintf(stdout, "%lld\n", MyDu(argv[1]) / 2);

    	exit(0);
}

static int64_t MyDu(const char *path){
	int64_t sum;
	char next[MAXSIZE];
	struct stat filestat;
	glob_t globres;
	if (lstat(path, &filestat) < 0){
		perror("lstat error");
		exit(1);
	}

	if (!S_ISDIR(filestat.st_mode)){
		return filestat.st_blocks;
	}

	strncpy(next, path, MAXSIZE);
	strncat(next, "/*", MAXSIZE);
	glob(next, 0, nullptr, &globres);

	strncpy(next, path, MAXSIZE);
	strncat(next, "/.*", MAXSIZE);
	glob(next, GLOB_APPEND, nullptr, &globres);

	sum = filestat.st_blocks;

	for (auto i = 0; i < globres.gl_pathc; i++){
		if (!IsPoint(globres.gl_pathv[i])){
			sum += MyDu(globres.gl_pathv[i]);
		}
	}

	globfree(&globres);

	return sum;
}

static bool IsPoint(char *path){
	char *p = nullptr;
	p = strrchr(path, '/');
	if (p == nullptr){
		exit(1);
	}

	if (strcmp(p+1, ".") == 0 || strcmp(p+1, "..") == 0){
		return true;
	}

	return false;
}
