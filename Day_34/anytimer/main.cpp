#include <cstdio>
#include <cstring>
#include <csignal>
#include "anytimer.h"

void f1(void *ptr){
    printf("f1():%s\n", (char *)ptr);
}

//void f2(void *ptr){
//    printf("f2():%s\n", (char *)ptr);
//}

int main() {
    int job1;

    puts("Begin!");
    job1 = anytimer::at_repeatjob(2, f1, (void *)"aaa");
    if (job1 < 0){
        fprintf(stderr, "at_addjob()%s\n", strerror(-job1));
    }
    // job2 = anytimer::at_addjob(2, f2, (void *)"bbb");
    // job3 = anytimer::at_addjob(7, f1, (void *)"ccc");

    puts("End!");

    while (true){
        write(1, ".", 1);
        sleep(1);
    }

}

