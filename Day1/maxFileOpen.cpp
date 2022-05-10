#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(){
	int cnt = 0;
	FILE* fp = nullptr;
	while(1){
		if ((fp = fopen("note", "r")) == nullptr){
			perror("open error");
			break;
		}
		cnt++;
	}
	printf("max open file: %d\n", cnt);
	exit(0);
}
