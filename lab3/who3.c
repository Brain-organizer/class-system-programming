#include <stdlib.h>
#include <time.h>
#include "utmplib.c"

void show_info(struct utmp *utbufp);

int main(){
	struct utmp *utbufp;

	if (utmp_open(UTMP_FILE) == -1) {
		perror(UTMP_FILE);
		exit(1);
	}
	
	while ((utbufp = utmp_next()) != NULLUT)
		show_info(utbufp);

	utmp_close();

	return 0;
}

void show_info(struct utmp* utbufp){
        if (utbufp->ut_type != USER_PROCESS)
		return;

	time_t t = utbufp->ut_tv.tv_sec;
        struct tm *btime = localtime(&t);

        printf("%-8.8s ", utbufp->ut_user);

        printf("%-12.12s ", utbufp->ut_line);

        printf("%04d-%02d-%02d %02d:%02d ", btime->tm_year + 1900, btime->tm_mon + 1, btime->tm_mday, btime->tm_hour, btime->tm_min);

        printf("(%s)", utbufp->ut_host);
        printf("\n");
}
