#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int option = 0;
int remain = 1;
int cnt = 0;

void helloworld();

int main(int argc, char* argv[]){
	if(argc>3){
		printf("too many arguments\nUsage: %s [-n positive_integer]\n",argv[0]);
	}
	else if(argc == 1){
		helloworld();
	}
	else if(strcmp(argv[1],"-n") == 0){
		char limit[100];

		if(argc==2){
			printf("Please specify the limit of HelloWorld! to print: ");
			fgets(limit,sizeof(limit),stdin);
		}
		else{
			strncpy(limit,argv[2],99);
			limit[99]='\0';
		}

		if (atoi(limit) <= 0){
			printf("Invalid input. Please enter a positive integer.\n");
			return 1;
		}
		
		remain = atoi(limit);
		option = 1;
		helloworld();

	}
	else{
		printf("Invalid option\nUsage: %s [-n positive_integer]\n",argv[0]);
	}
}

void f(int signum){
	printf("\n=====================================================\n");
	printf("Total number of printed HelloWorld!: %d\n", cnt);
	if(option ==1)
		printf("Number of HelloWorld! prints remaining: %d\n", remain);
	printf("=====================================================\n");

	if(cnt%2 == 1)
		exit(0);
}


void helloworld(){
	void f(int);	
	
	signal(SIGINT, f);
	while(remain){
		printf("HelloWorld!\n");
		cnt++;
		if(option)
			remain--;
		sleep(1);
	}
	
	printf("=====================================================\n");
        printf("Total number of printed HelloWorld!: %d\n", cnt);
        printf("Number of HelloWorld! prints remaining: %d\n", remain);
        printf("=====================================================\n");

}
