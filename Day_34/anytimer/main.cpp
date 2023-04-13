#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "anytimer.h"

void f1(void *ptr){

}

void f2(void *ptr){

}

int main() {
    int job1, job2, job3;

    puts("Begin!");
    job1 = anytimer::at_addjob(5, f1, (void *)"aaa");
    if (job1 < 0){
        fprintf(stderr, "at_addjob()%s\n", strerror(-job1));
    }
    job2 = anytimer::at_addjob(2, f2, (void *)"bbb");
    job3 = anytimer::at_addjob(7, f1, (void *)"ccc");
    puts("End!");

}

