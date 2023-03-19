#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <unistd.h>

int main(){
	uid_t uid = 1000;
	seteuid(uid);

	fprintf(stdout, "user real permission is %d and effective permission is %d\n", getuid(), geteuid());
	exit(0);
}

