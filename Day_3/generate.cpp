#include <cstdio>
#include <cstdlib>


int main(){
	FILE *fp;
	int i = 1;

	fp = fopen("./test_file_1", "w");
	for (;i <= 20; i++){
		fprintf(fp, "this is line %d\n", i);
	}

	exit(0);
}
