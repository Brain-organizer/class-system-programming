#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *s1, char *s2);

int main(int argc, char* argv[]){
	int in_fd, out_fd, n_chars;
	char buf[BUFFERSIZE];

	if(argc!=3){
		fprintf(stderr, "usage:%s source destination\n", *argv);
		exit(1);
	}

	if ((in_fd = open(argv[1], O_RDONLY)) == -1)
		oops("Cannot open ", argv[1]);

	if ((out_fd = creat(argv[2], COPYMODE)) == -1)
		oops("Cannot creat ", argv[2]);
	
	while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
		if (write(out_fd, buf, n_chars) != n_chars)
			oops("write error to", argv[2]);
	if (n_chars == -1)
		oops("read error from", argv[1]);

	if (close(in_fd) == -1 || close(out_fd) == -1)
		oops("error closing file", "");
}

void oops(char *s1, char *s2) {
	fprintf(stderr, "error: %s", s1);
	perror(s2);
	exit(1);
}
