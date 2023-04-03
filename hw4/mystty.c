#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define AMODE 0
#define IMODE 1
#define OMODE 2
#define CMODE 3
#define LMODE 4

struct flaginfo{
	int fl_value;
	char *fl_name;
};

struct flaginfo input_flags[] = {
	IGNBRK, "ignbrk",
	BRKINT, "brkint",
	IGNPAR, "ignpar",
	PARMRK, "parmrk",
	INPCK,	"inpck",
	ISTRIP, "istrip",
	INLCR, 	"inlcr",
	IGNCR,	"igncr",
	ICRNL,	"icrnl",
	IXON,	"ixon",
	IXOFF,	"ixoff",
	0,	NULL
};

struct flaginfo local_flags[] = {
	ISIG, "isig",
	ICANON, "icanon",
	ECHO, "echo",
	ECHOE, "echoe",
	ECHOK, "echok",	
	0,	NULL
};

struct flaginfo output_flags[] = {
	OLCUC, "olcuc"
};

void showbaud(int thespeed);
void show_some_flags(struct termios *ttyp);
void show_flagset(int thevalue, struct flaginfo thebitnames[]);
void do_change_flags(struct termios *ttyp, char** strflags, int mode);
void change_flags(tcflag_t * c_flagp, char **strflags, struct flaginfo flag_list[]);
void flip_flags(struct termios *ttyp, int mode);
int get_flag(struct flaginfo flag_list[], char* strflag);

int main(int argc, char *argv[]){
	struct termios ttyinfo;
	
	if(tcgetattr(0, &ttyinfo) == -1){
		perror("cannot get params about stdin");
		exit(1);
	}

	if(argc>1){
		if(strcmp("-FI",argv[1])==0)
			do_change_flags(&ttyinfo, argv+2, IMODE);
		else if(strcmp("-FO",argv[1])==0)
			flip_flags(&ttyinfo, OMODE);
		else if(strcmp("-FL",argv[1])==0)
			do_change_flags(&ttyinfo, argv+2, LMODE);
		else if(strcmp("-A", argv[1])==0)
			flip_flags(&ttyinfo, AMODE);
		else{
			fprintf(stderr, "invalid option or argument %s\n", argv[1]);
			printf("Usage: %s [-FI |-FO |-FL |-A] [flag]\n",argv[0]);
			exit(1);
		}
	}	
	tcsetattr(0, TCSANOW, &ttyinfo);

	showbaud(cfgetospeed(&ttyinfo));
	printf("erase = ^%c;kill = ^%c;\n", ttyinfo.c_cc[VERASE]-1+'A', ttyinfo.c_cc[VKILL]-1+'A');
	show_some_flags(&ttyinfo);
}

void do_change_flags(struct termios *ttyp, char** strflags, int mode){
	int flag;
	switch(mode){
	case IMODE:
		change_flags(&(ttyp->c_iflag),strflags,input_flags); break;
	case OMODE:
		change_flags(&(ttyp->c_oflag),strflags,output_flags);break;
	case LMODE:
		change_flags(&(ttyp->c_lflag),strflags,local_flags);break;
	}
}

void change_flags(tcflag_t * c_flagp, char **strflags, struct flaginfo flag_list[]){
	int flag, i;
	for(i =0; strflags[i]; i++)
        	if((flag = get_flag(flag_list,strflags[i])) != -1)
                        *c_flagp ^= flag;
                else
                        break;
        if(flag==-1){
                for(i-- ;i>=0; i--)
                        *c_flagp ^= get_flag(flag_list,strflags[i]);
                exit(1);
        }
}


int get_flag(struct flaginfo flag_list[], char* strflag){
        for(int i =0; flag_list[i].fl_value ; i++)
		if(strcmp(strflag,flag_list[i].fl_name) ==0)
			return flag_list[i].fl_value;

	fprintf(stderr,"%s :flag does not exist\n",strflag);
	return -1;
}

void flip_flags(struct termios *ttyp, int mode){
	if(mode ==AMODE || mode == IMODE)
		for(int i=0; input_flags[i].fl_value ; i++)
			ttyp->c_iflag ^= input_flags[i].fl_value;
	
	if(mode == AMODE || mode == OMODE)
		for(int i=0; output_flags[i].fl_value ; i++)
                        ttyp->c_oflag ^= output_flags[i].fl_value;

	if(mode == AMODE || mode == LMODE)
                for(int i=0; local_flags[i].fl_value ; i++)
                        ttyp->c_lflag ^= local_flags[i].fl_value;
}

void showbaud(int thespeed){
	printf("speed ");
	switch( thespeed ){
		case B300:	printf("300"); break;
		case B600:      printf("600"); break;
		case B1200:      printf("1200"); break;
		case B1800:      printf("1800"); break;
		case B2400:      printf("2400"); break;
		case B4800:      printf("4800"); break;
		case B9600:      printf("9600"); break;
		default:      printf("Fast"); break;
	}
	printf(" baud;\n");
}

void show_some_flags(struct termios *ttyp){
	show_flagset(ttyp->c_iflag, input_flags);
	show_flagset(ttyp->c_oflag, output_flags);
	show_flagset(ttyp->c_lflag, local_flags);
}

void show_flagset(int thevalue, struct flaginfo thebitnames[]){
	int i;
	for (i=0; thebitnames[i].fl_value; i++){
		if(thevalue & thebitnames[i].fl_value)
			printf("%s ", thebitnames[i].fl_name);
		else
			printf("-%s ", thebitnames[i].fl_name);
	}
	printf("\n");
}

