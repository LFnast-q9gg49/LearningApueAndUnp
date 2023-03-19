// gmtime
//

#include <cstdio>
#include <cstdlib>

#include <time.h>
int main(){
	struct tm *tm_test = nullptr;
	tm_test = (struct tm*)malloc(sizeof(struct tm));
	free(tm_test);
}
