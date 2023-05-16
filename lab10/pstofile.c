#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void main(){
    int pid;
    int fd;
    
    printf("About to run ps aux into a file\n");
    if((pid = fork()) == -1){
        perror("fork"); exit(1);
    }
    else if(pid == 0){
        close(1);
        fd = creat("processlist", 0644);
        execlp("ps", "ps", "aux", NULL);
        perror("execlpo"); exit(1);
    }
    else{
        wait(NULL);
        printf("Done running ps aux. results in processlist\n");
    }
    return;
}