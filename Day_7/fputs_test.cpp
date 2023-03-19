# include <cstdio>
# include <cstdlib>

const int BUFSIZE = 4;

int main(){
	FILE *fp = nullptr;
	char buf[BUFSIZE];
	const char *f_path = "./test";

	fp = fopen(f_path, "r");
	// err
	
	while(fgets(buf, BUFSIZE, fp) != nullptr){
		// err
		fputs(buf, stdout);
		fputc('\t', stdout);
	}
	exit(0);
}
