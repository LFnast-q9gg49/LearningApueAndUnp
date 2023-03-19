#include <cstdio>
#include <cstdlib>
#include <signal.h>

int main(){
	int i = 0;

	auto sig_handler = [](int)->void{
		write(1, "!", 1);
	};

	signal(SIGINT, sig_handler);

	sigset_t set, old_set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);

	for (auto j = 0; j < 1000; j++){
		sigprocmask(SIG_BLOCK, &set, &old_set);
		for (i = 0;i < 5; i++){
			write(1, "*", 1);
			sleep(1);
		}
		sigprocmask(SIG_SETMASK, &old_set, nullptr);
		write(1, "\n", 1);
	}
	exit(0);
}
