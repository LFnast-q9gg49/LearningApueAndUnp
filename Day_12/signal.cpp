#include <cstdio>
#include <cstdlib>
#include <signal.h>

int main(){
	int i = 0;
	
	auto sig_handler = [](int)-> void{fprintf(stdout, "receive interut\n");};

	signal(SIGINT, sig_handler);

	for (; i < 10; i++){
		write(1, "*", 1);
		sleep(1);
	}

	exit(0);
}
