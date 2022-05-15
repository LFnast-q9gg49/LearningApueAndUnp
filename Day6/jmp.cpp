#include<stdio.h>
#include<stdlib.h>
#include<setjmp.h>

static void a(void);
static void b(void);
static void c(void);
static void d(void);
static jmp_buf sp;

int main(){
		printf("%s():main Begin.\n", __FUNCTION__);
		printf("%s():Call a().\n", __FUNCTION__);
		a();
		printf("%s():a() return.\n", __FUNCTION__);
		printf("%s():main End.\n", __FUNCTION__);
}

void a(void){
        int flag = 0;
		printf("%s():a Begin.\n", __FUNCTION__);
        flag = setjmp(sp);
		if (flag == 0){
			printf("%s():Call b().\n", __FUNCTION__);
        	b();
			printf("%s():b() return.\n", __FUNCTION__);
		}else{
				fprintf(stdout, "%s():Jumped back with code%d\n", __FUNCTION__, flag);
		}

		printf("%s():a End.\n", __FUNCTION__);
}

void b(void){
        printf("%s():b Begin.\n", __FUNCTION__);
        printf("%s():Call c().\n", __FUNCTION__);
        c();
		printf("%s():c() return.\n", __FUNCTION__);
        printf("%s():b End.\n", __FUNCTION__);
}

void c(void){
        printf("%s():c Begin.\n", __FUNCTION__);
        printf("%s():Call d().\n", __FUNCTION__);
        d();
        printf("%s():d() return.\n", __FUNCTION__);
        printf("%s():c End.\n", __FUNCTION__);
}

void d(void){
        printf("%s():d Begin.\n", __FUNCTION__);
        
		fprintf(stdout, "%s():Ready to jump\n", __FUNCTION__);
		longjmp(sp, 0);

		printf("%s():d End.\n", __FUNCTION__);
}
