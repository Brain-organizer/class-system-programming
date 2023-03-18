#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

struct fileinfo{
	char* filename;
	struct stat info;
};

#define FISIZE sizeof(struct fileinfo)

int n_fileinfo, list_size;

void do_ls (char[], int);
void show_file_info(char*, struct stat*);
void mode_to_letters(int, char[]);
char* uid_to_name(uid_t);
char* gid_to_name(gid_t);
struct fileinfo* init_fileinfo_list();
struct fileinfo* get_sorted_fileinfo_list(char dirname[], int inc);
struct fileinfo* append_fileinfo_list(struct fileinfo* fileinfo_list , char* filename);
int compare_inc(const void*, const void*);
int compare_dec(const void*, const void*);

int main(int argc, char* argv[]){
	if (argc == 1)
		do_ls(".", 0);
	
	else if (strcmp(argv[1], "-S") == 0)
		do_ls(".", -1);

	else if (strcmp(argv[1], "-s") == 0)
		do_ls(".", 1);
}

void do_ls(char dirname[], int inc){
	struct fileinfo* fileinfo_list = get_sorted_fileinfo_list(dirname, inc);

	for(int i=0; i<n_fileinfo; i++){
		show_file_info(fileinfo_list[i].filename, &(fileinfo_list[i].info));
	}
}

struct fileinfo* get_sorted_fileinfo_list(char dirname[], int inc){
	DIR *dir_ptr;
	struct dirent *direntp;
	char path[256];
	struct fileinfo* fileinfo_list = init_fileinfo_list();

	if (chdir(dirname) != 0) {
		perror(dirname);
		exit(1);
	}

	getcwd(path, sizeof(path));

	if ((dir_ptr = opendir(path)) == NULL)
		fprintf(stderr,"ls2: cannot open %s\n", path);
	else
		while ((direntp = readdir(dir_ptr)) != NULL)
			fileinfo_list = append_fileinfo_list(fileinfo_list, direntp->d_name);
	
	if (inc == 1)
		qsort(fileinfo_list, n_fileinfo, FISIZE, compare_inc);
	else if (inc ==-1)
		qsort(fileinfo_list, n_fileinfo, FISIZE, compare_dec);

	return fileinfo_list;
}

int compare_inc(const void* a, const void* b){
	int size1 = ((struct fileinfo*)a)->info.st_size;
	int size2 = ((struct fileinfo*)b)->info.st_size;

	if (size1<size2) return -1;
	if (size1>size2) return 1;
	return 0;
}

int compare_dec(const void* a, const void* b){
        int size1 = ((struct fileinfo*)a)->info.st_size;
        int size2 = ((struct fileinfo*)b)->info.st_size;

        if (size1>size2) return -1;
        if (size1<size2) return 1;
        return 0;
}


struct fileinfo* init_fileinfo_list(){
	n_fileinfo = 0;
	list_size = 1;

	return malloc(FISIZE * list_size);
}

struct fileinfo* append_fileinfo_list(struct fileinfo* fileinfo_list, char* filename){
	if (n_fileinfo == list_size){
		struct fileinfo* new_list = malloc(FISIZE * list_size * 2);
		
		memcpy(new_list, fileinfo_list, FISIZE * list_size);
		free(fileinfo_list);

		fileinfo_list = new_list;
		list_size *= 2;
	}

	if ( stat(filename, &(fileinfo_list[n_fileinfo].info)) == -1)
		perror(filename);
	else{
		fileinfo_list[n_fileinfo].filename = filename;
		n_fileinfo++;
	}
	
	return fileinfo_list;
}

void show_file_info(char* filename, struct stat* info_p){
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];

	mode_to_letters(info_p->st_mode, modestr);
	printf("%s", modestr);
	printf("%4d ", (int) info_p->st_nlink);
	printf("%-8s ", uid_to_name(info_p->st_uid));
	printf("%-8s ", gid_to_name(info_p->st_gid));
	printf("%-8ld ", (long)info_p->st_size);
	printf("%.12s ", 4+ctime(&info_p->st_mtime));
	printf("%s \n", filename);
}

void mode_to_letters(int mode, char str[]){
	strcpy(str, "----------");

	if (S_ISDIR(mode)) str[0] = 'd';
	if (S_ISCHR(mode)) str[0] = 'c';
	if (S_ISBLK(mode)) str[0] = 'b';

	if (mode & S_IRUSR) str[1] = 'r';
	if (mode & S_IWUSR) str[2] = 'w';
	if (mode & S_IXUSR) str[3] = 'x';

	if (mode & S_IRGRP) str[4] = 'r';
	if (mode & S_IWGRP) str[5] = 'w';
	if (mode & S_IXGRP) str[6] = 'x';

	if (mode & S_IROTH) str[7] = 'r';
	if (mode & S_IWOTH) str[8] = 'w';
	if (mode & S_IXOTH) str[9] = 'x';
}

char* uid_to_name(uid_t uid){
	struct passwd *getpwuid(), *pw_ptr;
	static char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL){
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

char* gid_to_name(gid_t gid){
	struct group *getgrid(), *grp_ptr;
	static char numstr[10];
	
	if ((grp_ptr = getgrgid(gid)) == NULL){
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}

