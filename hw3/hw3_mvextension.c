#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

void do_extmv(char*, char*);
void do_mv(char*, char*);
int dest_exist(char*);

int main(int argc, char* argv[]){
	int i = argc > 5 ? 5 : 3;
	if (argc==1 || strcmp(argv[1], "-d") != 0 || !dest_exist(argv[2])){
		fprintf(stderr, "Destination directory does not exist\n");
		exit(1);
	}

	if (argc < 5)
		fprintf(stderr, "%s: insufficient arguments\n" , argv[0]);
	else if (strcmp(argv[3], "-e") == 0 && argc == 5){
		do_extmv(argv[2], argv[4]);
		return 0;
	} else if (argv[i][0] == '-')
		fprintf(stderr, "%s: invalid option -- '%s'\n", argv[0], argv[i] + 1);
	else
		fprintf(stderr, "%s: invalid argument -- '%s'\n", argv[0], argv[i]);
	
	fprintf(stderr, "Usage: %s -d destination_directory -e extension\n", argv[0]);
	exit(1);
}

int dest_exist(char* dest){
	int fd;
	
	if ((fd = open(dest, O_RDONLY)) == -1){
		close(fd);
		return 0;
	}
	
	close(fd);
	return 1;
}

void do_extmv(char* dest, char* ext){
	DIR * dir_ptr;
	struct dirent *direntp;
	int extlen, fnamelen, n_extfile; 
	n_extfile = 0;
	extlen = strlen(ext);


	if ((dir_ptr = opendir(".")) == NULL){
		perror(".");
		exit(1);
	}

	while ((direntp = readdir(dir_ptr)) != NULL){
		fnamelen = strlen(direntp->d_name);
		
		if (extlen<fnamelen && direntp->d_name[fnamelen-extlen-1] == '.' && strcmp(direntp->d_name +(fnamelen-extlen), ext) == 0){
			do_mv(direntp->d_name, dest);
			n_extfile++;
		}
	}

	if(n_extfile == 0)
		fprintf(stderr, "No files with extension %s found\n", ext);
	
}

void do_mv(char* fname, char* dest){
	int fnamelen, destlen;
       	fnamelen = strlen(fname);
	destlen = strlen(dest);	
	
	char newpath[fnamelen+destlen+2];
	newpath[0] = '\0';
		
	strcat(newpath, dest);
	if(newpath[destlen-1] != '/')
		strcat(newpath, "/");
	strcat(newpath, fname);

	if(rename(fname, newpath) == -1){
		fprintf(stderr, "%s cannot move to %s\n", fname, newpath);
		perror("rename error");
	}
}	


