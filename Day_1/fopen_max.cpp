#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <string.h>

int main(int argc, char **argv){
	FILE *fp=nullptr;
	int cnt=0;
	for (;;){
		fp = fopen("./tmp", "r");
		if (fp == nullptr){
			fprintf(stderr, "fopen max error, %s\n", strerror(errno));
			break;
		}
		cnt -=-1;
	}
	fprintf(stdout, "count is %d\n", cnt);
	exit(0);
}
