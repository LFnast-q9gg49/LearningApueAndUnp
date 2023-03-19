#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc, char **argv){
	DIR *dire = nullptr;
	dirent *f_dirent;
	if (argc < 2){
		fprintf(stderr, "error occured : %s\n", strerror(errno));
		exit(1);
	}

	if ((dire = opendir(argv[1])) == nullptr){
		fprintf(stderr, "error occured : %s\n", strerror(errno));
		exit(1);
	}

	while((f_dirent = readdir(dire)) != nullptr){
		fputs(f_dirent->d_name, stdout);
		fprintf(stdout, "\n");
	}

	closedir(dire);

	exit(0);
}
