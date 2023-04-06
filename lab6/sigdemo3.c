#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int start_time=0;

void f(int signum){ 
	printf("Currently elapsed time: %d sec(s)\n", start_time);	
}

int main(){
	void f(int);
	int i;

	signal(SIGINT, f);
	printf("you can't stop me!\n");
	while(1){ 
		start_time++;
		printf("haha\n");
		sleep(1);
	}
}
