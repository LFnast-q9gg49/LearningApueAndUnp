#include<stdio.h>
#include<stdlib.h>

static void testFunc(void){
		puts("this is testFunc");
}

static void testFunc2(void){
		puts("this is testFunc2");
}

int main(int argc, char **argv){
		puts("Begin");
		atexit(testFunc);
		atexit(testFunc2);
		puts("Ends");
		exit(0);
}
