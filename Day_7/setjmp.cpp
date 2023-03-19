#include <cstdio>
#include <cstdlib>

#include <setjmp.h> 

static jmp_buf save;

static void d(void){
        fprintf(stdout, "%s():d Begin.\n", __FUNCTION__);
	fprintf(stdout, "%s():Ready To Jump.\n", __FUNCTION__);
	longjmp(save, 0);
        fprintf(stdout, "%s():d End.\n", __FUNCTION__);
}


static void c(void){
        fprintf(stdout, "%s():c Begin.\n", __FUNCTION__);

        fprintf(stdout, "%s():Call d().\n", __FUNCTION__);

        d();

        fprintf(stdout, "%s():d() Returned.\n", __FUNCTION__);

        fprintf(stdout, "%s():c End.\n", __FUNCTION__);
}


static void b(void){
        fprintf(stdout, "%s():b Begin.\n", __FUNCTION__);

        fprintf(stdout, "%s():Call c().\n", __FUNCTION__);

        c();

        fprintf(stdout, "%s():c() Returned.\n", __FUNCTION__);

        fprintf(stdout, "%s():b End.\n", __FUNCTION__);
}


static void a(void){
	int ret;

        fprintf(stdout, "%s():a Begin.\n", __FUNCTION__);
	ret = setjmp(save);
	if (ret == 0){
	
        	fprintf(stdout, "%s():Call b().\n", __FUNCTION__);
        	b();
        	fprintf(stdout, "%s():b() Returned.\n", __FUNCTION__);
	}else{
		fprintf(stdout, "%s():Jumped back here with code %d.\n", __FUNCTION__, ret);
	}
        fprintf(stdout, "%s():a End.\n", __FUNCTION__);
}


int main(){
	fprintf(stdout, "%s():Begin.\n", __FUNCTION__);

	fprintf(stdout, "%s():Call a().\n", __FUNCTION__);

	a();

	fprintf(stdout, "%s():a() Returned.\n", __FUNCTION__);

	fprintf(stdout, "%s():End.\n", __FUNCTION__);

	exit(0);
}
