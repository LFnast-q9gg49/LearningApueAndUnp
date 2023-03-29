// count chars of a file

#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
    int fd, i, count;
    struct stat statres{};
    void *mmap_ret;

    if (argc < 2){
        fprintf(stderr, "Usage : ./mmap <file_name>\n");
        exit(1);
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0){
        fprintf(stderr, "open\n");
        exit(1);
    }

    if (fstat(fd, &statres) < 0){
        fprintf(stderr, "stat\n");
        exit(1);
    }

    mmap_ret = mmap(nullptr, statres.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (mmap_ret == MAP_FAILED){
        close(fd);
        perror("mmap\n");
        exit(1);
    }

    char *str = (char *)mmap_ret;

    for (i = 0; i < statres.st_size; i++){
        fprintf(stdout, "%c", str[i]);
        if (str[i] == 'a'){
      
            count++;
        }
    }
    fprintf(stdout, "file has %d 'a'\n", count);
    
    // if run ./mmap too fast, maybe count will get wrong num;

    close(fd);

    munmap(mmap_ret, statres.st_size);

    exit(0);
}
