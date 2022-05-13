#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

const char *pattern = "/etc";

int main(){
	DIR *dp;
	struct dirent *cur;

	dp = opendir(pattern);
	if (dp == nullptr){
		perror("open dir failed : ");
		exit(1);
	}

	while((cur = readdir(dp)) != nullptr){
		puts(cur->d_name);
	}

	closedir(dp);
	exit(0);
}
