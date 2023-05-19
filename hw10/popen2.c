#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFF_SIZE 100

FILE *popen2(const char *, const char *);
int execf(const char *fullcmd);
void handler(int signum);
void sighandle_proc();

int main(){
    FILE *pipein_fp, *pipeout_fp;
    char readbuf[BUFF_SIZE];
    
    sighandle_proc();

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGKILL, SIG_IGN);
    signal(SIGTERM, SIG_IGN);

    sleep(3);

    if((pipein_fp = popen2("ls","r")) == NULL){
        perror("popen2");
        exit(1);
    }

    if((pipeout_fp = popen2("sort -r","w")) == NULL){
        perror("popen2");
        exit(1);
    }

    while( fgets(readbuf, BUFF_SIZE, pipein_fp))
        fputs(readbuf, pipeout_fp);
    
    pclose(pipein_fp);
    pclose(pipeout_fp);
    sleep(10);

    return 0;
}

FILE *popen2(const char *command, const char *mode){
    int p[2];
    int pid, childio, childpipe, parentpipe;
    FILE *parentfp;
    
    if(pipe(p) == -1){
        return NULL;
    }


    if(strcmp(mode,"w") == 0){
        childio = 0;
        childpipe = p[0];
        parentpipe = p[1];
    }
    else if(strcmp(mode, "r") == 0){
        childio = 1;
        childpipe = p[1];
        parentpipe = p[0];
    }
    else{
        return NULL;
    }
    

    if((pid = fork()) == -1){
        return NULL;
    }
    else if(pid == 0){
        close(parentpipe);
        dup2(childpipe, childio);
        close(childpipe);
        execf(command);
        printf("exec error");
        exit(1);
    }
    else{
        close(childpipe);
        parentfp = fdopen(parentpipe,mode);
        return parentfp;
    }
    
}

int execf(const char *fullcmd){
    char *cmdcp = (char *)malloc(strlen(fullcmd)+1);
    char **cmdv = (char **)malloc((strlen(fullcmd)/2 +1) * sizeof(char*));
    strcpy(cmdcp, fullcmd);
    int i = 0;

    cmdv[i] = strtok(cmdcp," ");
    while(cmdv[i++] != NULL){
        cmdv[i] != strtok(NULL," ");
    }

    return execvp(cmdv[0],cmdv);
}

void handler(int signum){
    switch (signum){
        case SIGINT:
            printf("\tSIGINT received but ignored...\n"); break;
        case SIGQUIT:
            printf("\tSIGQUIT received but ignored...\n"); break;
        case SIGKILL:
            printf("\tSIGKILL received but ignored...\n"); break;
        case SIGTERM:
            printf("\tSIGTERM received but ignored...\n"); break;
        case SIGCHLD:
            exit(0);
    }
}

void sighandle_proc(){
    if(fork()){
        printf("PID: %d\n", getpid());

        signal(SIGINT, handler);
        signal(SIGQUIT, handler);
        signal(SIGKILL, handler);
        signal(SIGTERM, handler);
        signal(SIGCHLD, handler);
        while(1){
            ;
        }
    }
}