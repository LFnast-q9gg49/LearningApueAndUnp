#include <cstdio>
#include <cstdlib>

#include <time.h>

int main(int argc, char **argv){
	time_t stamp;

	fprintf(stdout, "%ld\n", time(&stamp));
	fprintf(stdout, "%ld\n", stamp);
}
