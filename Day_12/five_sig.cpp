#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <functional>

#include <unistd.h>
#include <signal.h>

static volatile int loop = 1;

int main(){
    alarm(5);
    int64_t count = 0;

    auto sig_handler = [](int) -> void {
        loop = 0;
    };

    signal(SIGALRM, sig_handler);

    while (loop) {
        count++;
    }

    fprintf(stdout, "count is %ld\n", count);

}

