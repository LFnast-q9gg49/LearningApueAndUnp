#include <cstdio>
#include <cstdlib>

extern char **environ; 

int main(){
	int i = 0;
	for(; environ[i] != nullptr; i++){
		fprintf(stdout, "%s\n", environ[i]);
	}
	exit(0);
}
