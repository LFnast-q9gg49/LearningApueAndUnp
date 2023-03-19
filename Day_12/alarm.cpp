#include <cstdio>
#include <cstdlib>
#include <unistd.h>

int main(){
	alarm(5);
	alarm(1);
	

	while(1){
		pause();
	}


	exit(0);
}
