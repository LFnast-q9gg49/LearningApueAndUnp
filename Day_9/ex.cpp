#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <unistd.h>

// date

int main(){
	fputs("Begin\n", stdout);
	int ret;

	
	execl("/bin/date", "date", "+%s", nullptr);
	perror("execl()");
	exit(1);
	fputs("End\n", stdout);
	exit(0);
}
