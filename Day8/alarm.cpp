#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>
static int64_t func_one(){
		time_t end;
		int64_t count = 0;
		end = time(nullptr)+5;
		while(time(nullptr) <= end){
				count++;
		}
		return count;
}

static volatile int loop = 1;

static void alrm_handler(int s){
		loop = 0;
}

static int64_t func_two(){
		int64_t count;
		alarm(5);
		signal(SIGALRM, alrm_handler);
		while(loop){
				count++;
		}
		return count;
}

int main(){
		// fprintf(stdout, "res1:%ld\n", func_one());
		fprintf(stdout, "res2:%ld\n", func_two());
		exit(0);
}
