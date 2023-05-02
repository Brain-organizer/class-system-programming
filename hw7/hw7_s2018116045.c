#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int prodalldigit(int num);
int sumalldigit(int num);
void childproc(int cnt);

int main(){
    int n_proc, pid;

    printf("Enter the number of child processes: ");
    scanf("%d", &n_proc);

    for(int i=1; i<=n_proc; i++){
        pid = fork();
        
        if(pid == -1){
            perror("fork error");
            return -1;
        }
        else if(pid){
            wait(NULL);
        }
        else{
            childproc(i);
            exit(0);
        }
    }
    printf("All child processes completed.\n");

    return 0;
}

void childproc(int cnt){
    int pid, result;
    char *msg;

    pid = getpid();
    msg = pid%2 ? "odd" : "even";
    result = pid%2 ? prodalldigit(pid) : sumalldigit(pid);

    printf("Child process %d [PID - %d]: result(%s) = %d\n", cnt, pid, msg, result);
}

int prodalldigit(int num){
    int retval,digit; 

    retval = 1;
    while(num>0){
        digit = num%10;

        if(digit == 0)
            digit = 1;
        
        retval *= digit;

        num /= 10;
    }

    return retval;
}

int sumalldigit(int num){
    int retval,digit; 

    retval = 0;
    while(num>0){
        digit = num%10;

        retval += digit;

        num /= 10;
    }

    return retval;
}
