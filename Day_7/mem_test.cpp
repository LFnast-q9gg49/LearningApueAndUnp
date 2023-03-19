#include <cstdio>
#include <cstdlib>

int main(){
	fprintf(stdout, "name is %s\n", getenv("USER"));
	getchar();

	exit(0);
}
