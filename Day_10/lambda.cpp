// test lambda function
//
#include <cstdio>
#include <cstdlib>
#include <typeinfo>


int main(){
	int (*)(int, int) plus = [](int v1, int v2) -> int {return v1 + v2;};
	int sum = plus(1, 2);
	fprintf(stdout, "sum is %d\n", sum);
	fprintf(stdout, "type is %s\n", typeid(plus).name());
	exit(0);	
}

// [] () __ -> ret {} 
// capture, param, mutable, return and function
