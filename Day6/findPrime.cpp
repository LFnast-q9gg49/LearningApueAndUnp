#include<stdio.h>
#include<stdlib.h>

const int start = 30000000;
const int end   = 30000200;

int main(){
		int mark;
		for (auto i = start; i <= end; i++){
			mark = 1;
			for (auto j = 2; j < i/2; j++){
				if (i%j == 0){
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
