#include <cstdio>
#include <cstdlib>

int main(){
	// dup relocation
	//
	
	close(1);
	open(FNAME, O_WRONLY|O_TRUNC|O_CREAT, 0600);
	if (fd < 0){
		perror("error");
		exit(1);
	}
	exit(0);
}
