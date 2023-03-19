#include <cstdio>
#include <cstdlib>

#include <unistd.h>

int main(){

	fputc('a', stdout);
	write(1, "b", 1);

	fputc('a', stdout);
        write(1, "b", 1);

	fputc('a', stdout);
        write(1, "b", 1);

	exit(0);
}
