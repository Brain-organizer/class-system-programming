#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

time_t start_time; 

void f(int signum){ 
	printf("Currently elapsed time: %d sec(s)\n", (int)(time(NULL)-start_time));	
}

int main(){
	void f(int);
	int i;

	start_time = time(NULL);
	signal(SIGINT, f);
	printf("you can't stop me!\n");
	while(1){ 
		sleep(1);
		printf("haha\n");
	}
}
