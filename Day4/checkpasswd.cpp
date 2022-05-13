#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>
#include <string.h>
#include <shadow.h>
#include <unistd.h>

int main(int argc, char **argv){
	char *input;
	char *crypted_passwd;
	struct spwd *shadow;

	if (argc < 2){
		fprintf(stdout, "Usage error : ./checkpasswd <uname>\n");
		exit(1);
	}
	input = getpass("please input passwd : ");

	shadow = getspnam(argv[1]);

	crypted_passwd = crypt(input, shadow->sp_pwdp);

	if (strcmp(crypted_passwd, shadow->sp_pwdp) == 0){
		fprintf(stdout, "yes\n");
	}else{
		fprintf(stdout, "failed\n");
	}


	exit(0);
}
