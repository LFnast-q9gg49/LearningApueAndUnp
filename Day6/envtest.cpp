#include<stdio.h>
#include<stdlib.h>

int main(){
		fprintf(stdout, getenv("PATH"));
		fprintf(stdout, "\n");

		exit(0);
}
