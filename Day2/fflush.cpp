#include <stdio.h>
#include <stdlib.h>

int main(){
	/* Buffer used to merge system call
	 * line flush: '\n' eg.stdout
	 * full flush: default 
	 * none flush: stderror
	 *                        setvbuf();
	 * */

	int i = 0;
	fprintf(stdout, "Before while");
	while(1);
	fprintf(stdout, "After while");
	exit(0);
}
