// this file aims to delete the 10th line of a file
//

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char **argv){
	FILE *fp = nullptr, *fp_copy = nullptr;
	int tmp = 0, cnt = 0;
	off_t off_1 = 0, off_2 = 0;


	if (argc < 2){
		fprintf(stderr, "Usage ...\n");
		exit(1);
	}

	if ((fp = fopen(argv[1], "r+")) == nullptr){
		fprintf(stderr, "file open error : %s\n", strerror(errno));
		exit(1);
	}
	if ((fp_copy = fopen(argv[1], "r")) == nullptr){
                fprintf(stderr, "file open error : %s\n", strerror(errno));
                exit(1);
        }

	while((tmp = fgetc(fp)) != EOF){
		if (cnt == 9){
			off_1 = ftello(fp);
			break;
		}
		if (tmp == '\n'){
			cnt++;
		}
		
	}
	cnt = 0;
	rewind(fp);
	while((tmp = fgetc(fp)) != EOF){
                if (cnt == 10){
                        off_2 = ftello(fp);
			break;
                }
                if (tmp == '\n'){
                        cnt++;
                }

        }


	if (cnt < 10){
		fprintf(stderr, "File doesn't have enough lines\n");
		exit(1);
	}
	rewind(fp);

	fseeko(fp, off_1, SEEK_SET);
	fseeko(fp_copy, off_2, SEEK_SET);

	while((tmp = fgetc(fp_copy)) != EOF){
		fputc(tmp, fp);
	}
	fflush(fp);
	cnt = fileno(fp);
	ftruncate(cnt, ftello(fp));

	fclose(fp);
	fclose(fp_copy);


	exit(0);
}
