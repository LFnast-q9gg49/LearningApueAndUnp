#include <cstdio>
#include <cstdlib>
#include <time.h>

int main(){
	time_t end;
	int64_t count = 0;

	end = time(nullptr)+5;

	while(time(nullptr) <= end){
		count++;
	}

	fprintf(stdout, "count is %ld\n", count);
	exit(0);
}
