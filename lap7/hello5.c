#include <stdio.h>
#include <unistd.h>
#include <curses.h>

#define LEFTEDGE	10
#define RIGHTEDGE	30
#define ROW		10

int main(int argc, char *argv[]){
	char message[] = "s2018116045 - AnYongjin";
	char blank[] = "                       ";
	int dir = 1;
	int pos = LEFTEDGE;

	initscr();
	clear();
	while(1){
		move(ROW, pos);
		addstr(message);
		move(LINES-1, COLS-1);
		refresh();
		usleep(1500000);
		move(ROW, pos);
		addstr(blank);
		pos += dir;
		if(pos >= RIGHTEDGE){
			dir -= 1;
		}
		if(pos<=LEFTEDGE){
			dir += 1;
		}
	}
}
