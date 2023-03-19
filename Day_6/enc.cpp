#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <shadow.h>
#include <unistd.h>

int main(int argc, char **argv){
	char *input = nullptr;
	char *crypted = nullptr;
	struct spwd *sdline;
	if (argc < 2){
		fprintf(stderr, "Usage ...\n");
		exit(1);
	}

	input = getpass("Passwd : ");// not showing input in shell
	
	sdline = getspnam(argv[1]);

	crypted = crypt(input, sdline->sp_pwdp);

	if(strcmp(crypted, sdline->sp_pwdp) == 0){
		fprintf(stdout, "OK\n");
	}else{
		fprintf(stdout, "NO\n");
	}

	exit(0);
}
