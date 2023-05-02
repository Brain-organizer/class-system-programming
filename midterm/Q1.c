#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#define BUFFERSIZE 4096
#define COPYMODE 0700

void checkdest(char *dest, int opti){
    int destfd;
    char yon;
    if((destfd = open(dest,O_RDONLY)) != -1){
        if(opti){
            printf("파일을 덮어 쓰시겠습니까? (y/n) : ");
            if((yon = getc(stdin)) == 'n')
                exit(0);
        }
        close(destfd);

        unlink(dest);
    }
}

int getmod(char *filename){
    struct stat info;
    if(stat(filename, &info) == -1){
        perror(filename);
        exit(1);
    }

    return info.st_mode;
}

void optcv(char *src, char *dest, int opti, int optp){
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];
    int mod;

    if(opti){
        checkdest(dest,opti);
    }

    if((in_fd = open(src, O_RDONLY)) == -1){
        perror("source file open error");
        exit(1);
    }

    
    if((out_fd = creat(dest, COPYMODE)) == -1){
        perror("destination file open error");
        exit(1);
    }

    if(optp){
        chmod(dest,getmod(src));
    }


    while ((n_chars = read(in_fd, buf, BUFFERSIZE))>0){
        if(write(out_fd, buf, n_chars) != n_chars){
            perror("write error");
            exit(1);
        }
    }

    if(n_chars == -1){
        perror("read error");
        exit(1);
    }

    if (close(in_fd) == -1 || close(out_fd) == -1){
        perror("file close error");
        exit(1);
    }
}

void main(int argc, char* argv[]){
    int opti, optp, where;
    char *src, *dest;

    opti = optp = where = 0;
    src = dest = (char*)NULL;

    if (argc > 4){
        fprintf(stderr, "arguments error");
        return;
    }
    
    for (int i = 1; i< argc; i++){
        if(strcmp(argv[i],"-I")==0){
            opti = 1;
            where = i;
        }
        else if(strcmp(argv[i],"-IP") ==0 || strcmp(argv[i],"-PI") == 0){
            opti = optp = 1;
            where = i;
        }
        else if(strcmp(argv[i],"-P")==0){
            optp = 1;
            where = i;
        }
    }

    for(int i = 1; i<argc; i++){
        if(where==i)
            continue;

        if(src == NULL){
            src = argv[i];
        }
        else
            dest = argv[i];
    }

    optcv(src,dest,opti,optp);
}