#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    int lspipe[2];
    int pid, len;
    char keyword[200];

    printf("Enter a search string: ");
    fgets(keyword,200,stdin);
    len = strlen(keyword);
    keyword[len-1] = 0;
    
    if(pipe(lspipe) == -1){
        perror("couldn't make pipe");
        exit(1);
    }

    if((pid = fork()) == -1){
        perror("fork failed!");
        exit(1);
    }
    else if(pid == 0){
        if(dup2(lspipe[0],0) != 0){
            perror("dup2 failed");
            exit(1);
        }
        close(lspipe[0]);
        close(lspipe[1]);

        execlp("grep", "grep", keyword, NULL);
        printf("grep execution error\n");
    }
    else{
        if(dup2(lspipe[1],1) != 1){
            perror("dup2 failed");
            exit(1);
        }
        close(lspipe[0]);
        close(lspipe[1]);

        execlp("ls", "ls", NULL);
        printf("ls execution error\n");
    }

}