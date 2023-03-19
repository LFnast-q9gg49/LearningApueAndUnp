#include <cstdio>
#include <cstdlib>


int main(){
	static char *get = nullptr;
	
	get = getenv("USER");

	fprintf(stdout, "%s\n", get);

	exit(0);
}
