#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv){
	char *value;
	const char *key = "USER";
	const char *new_value = "nmpas";

	fprintf(stdout, "user is %s, and address is %ld\n", getenv(key), getenv(key));
	
	setenv(key, new_value, 1);
	

        fprintf(stdout, "user is %s, and address is %ld\n", getenv(key), getenv(key));

	getchar();
	exit(0);


}
