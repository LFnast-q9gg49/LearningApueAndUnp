#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

int main(){
	fprintf(stdout, "%d, %d, %d\n", fileno(stdin), fileno(stdout), fileno(stderr));
	
	exit(0);
}
