#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void show_info(struct utmp* utbufp);

int main(){
	struct utmp current_record;
	int reclen = sizeof(current_record);
	int utmpfd;

	if((utmpfd=open(UTMP_FILE, O_RDONLY))==-1){
		perror(UTMP_FILE);
		exit(1);
	}

	while(read(utmpfd,&current_record,reclen)==reclen)
		if(current_record.ut_type==USER_PROCESS)
			show_info(&current_record);
	close(utmpfd);
	return 0;
}

void show_info(struct utmp* utbufp){
	time_t t = utbufp->ut_tv.tv_sec;
	struct tm *btime = localtime(&t);

	printf("%-8.8s ", utbufp->ut_user);

	printf("%-12.12s ", utbufp->ut_line);

	printf("%04d-%02d-%02d %02d:%02d ", btime->tm_year + 1900, btime->tm_mon + 1, btime->tm_mday, btime->tm_hour, btime->tm_min);

	printf("(%s)", utbufp->ut_host);
	printf("\n");
}
