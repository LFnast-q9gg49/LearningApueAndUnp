#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <signal.h>

int main(){
	alarm(5);
	// set 5 seconds
	
	auto signal_handler = [] (int) -> void {
		fprintf(stdout, "5 seconds passed\n");
		exit(0);
	};

	signal(SIGALRM, signal_handler);

	for(;;);
}
