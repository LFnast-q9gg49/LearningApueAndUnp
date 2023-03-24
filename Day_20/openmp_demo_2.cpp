#include <cstdio>
#include <cstdlib>
#include <omp.h>

int main(){
#pragma omp parallel sections
	{
#pragma omp section
		fprintf(stdout, "[%d]Hello\n", omp_get_thread_num());
#pragma omp section
		fprintf(stdout, "[%d]World\n", omp_get_thread_num());
	}
	exit(0);
}
