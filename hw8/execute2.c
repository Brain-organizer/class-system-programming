/*
 * code used by small shell to execute commands
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "varlib.h"

extern char **environ;

int cpids[100];
int childnum = 0;

int is_nonblocking();

int killchild(){
    for(int i=0; i<childnum; i++){
        kill(cpids[i], SIGKILL);
    }
}

// run a program passing it arguments
// return status via wait, or -1 on error
// -1 on fork() or wait() errors
int execute(char* argv[]){
    int pid;
    int child_info = -1;

    if( argv[0] == NULL )
        return 0;
    if((pid = fork()) == -1)
        perror("fork");
    else if(pid == 0){
        environ = VLtable2environ();
        
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        execvp(argv[0], argv);
        perror("cannot execute command");
        exit(1);
    }
    else{
        cpids[childnum++] = pid;

        if(!is_nonblocking()){
            if(waitpid(pid, &child_info, 0) == -1)
                perror("wait");
        }
        else{
            printf("PID : %d\n", pid);
        }
    }
    return child_info;
}