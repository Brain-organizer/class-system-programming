#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>

void ignore(int signum){
    printf("\nSIGINT signal received. Ignoring...\n");
}

void charerr(int signum){
    printf("\nillegal number entered!\n SIGUSR1 signal accept! Exit Program.\n");
    exit(0);
}

void div0(int signum){
    printf("\nDenominator is 0!!\nSIGFPE signal accept! Exit Program.\n");
    exit(0);
}

void expire(int signum){
    printf("\n5 seconds have passed. Program will now exit.\n");
    exit(0);
}

int main(){
    char buf[1024];
    float op1, op2, final;
    char operator;
    int fail = 0;

    final = 0;
    signal(SIGINT, ignore);
    signal(SIGUSR1, charerr);
    signal(SIGFPE, div0);
    signal(SIGALRM, expire);

    while(1){
        printf("Enter the first number: ");
        scanf("%s", buf);
        op1 = atof(buf);
        if(buf[0]!='0' && op1==0){
            if(kill(getpid(),SIGUSR1) == -1){
                perror("kill error");
                exit(1);
            }
        }

        getchar();
        printf("Enter an operator (+,-,*,/): ");
        scanf("%c", &operator);

        printf("Enter the second number: ");
        scanf("%s", buf);
        op2 = atof(buf);
        if(buf[0]!='0' && op2==0){
            if(kill(getpid(),SIGUSR1) == -1){
                perror("kill error");
                exit(1);
            }
        }

        switch(operator){
            case '+': final = op1+op2; break;
            case '-': final = op1-op2; break;
            case '*': final = op1*op2; break;
            case '/': if(op2==0) kill(getpid(),SIGFPE) ;final = op1/op2; break;
            default: fail = 1;
        }

        if(fail){
            printf("invalid operator");
            exit(1);
        }

        printf("%.2f %c %.2f = %.2f",op1, operator, op2, final);
        printf("\nWould you like to calculate again? (Y/N): ");
        alarm(5);
        getchar();
        scanf("%c",&operator);
        if(!(operator == 'Y' || operator == 'y')){
            break;
        }
        else{
            alarm(0);
        }

    }
    
    
    
}