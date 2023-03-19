#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char **argv){
	struct passwd *pwdline = nullptr;
	if (argc < 2){
		fprintf(stderr, "Usage ... \n");
		exit(1);
	}

	pwdline = getpwuid(atoi(argv[1]));
	fprintf(stdout, "name is %s\n", pwdline->pw_name);

	exit(0);
}
