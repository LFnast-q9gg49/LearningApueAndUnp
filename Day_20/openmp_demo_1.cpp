#include <cstdio>
#include <cstdlib>

int main(){
#pragma omp parallel
	{
	puts("hello");
	puts("world");
}
	exit(0);
}
