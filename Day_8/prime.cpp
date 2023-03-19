#include <cstdio>
#include <cstdlib>

const int LEFT = 30000000;
const int RIGHT = 30000200;

int main(){
	int i, j ,mark;

	for (i = LEFT; i <= RIGHT; i++){
		mark = 1;

		for (j = 2; j < i/2; j++){
			if (i % j == 0){
				mark = 0;
				break;
			}
		}
		if (mark){
			fprintf(stdout, "%d is a primer\n", i);
		}
	}
	exit(0);
}
