/*
 * code used by small shell to execute commands
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "varlib.h"

extern char **environ;
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
        if(wait(&child_info) == -1)
            perror("wait");
    }
    return child_info;
}