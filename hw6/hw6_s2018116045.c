#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#define BLANK " "

int row, col, dirx, diry;
int trow, brow, ledg, redg;
int delay;
char *ball;

int set_ticker(int n_msecs){
    struct itimerval new_timeset;
    long n_sec, n_usecs;

    n_sec = n_msecs / 1000;
    n_usecs = (n_msecs % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;

    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void posx(){
    int sign;
    
    sign = dirx>0? 1 : -1;
    for(int i=0; i<(dirx*sign) ; i++){
        if ((col <= ledg + 1 && dirx<0) || (col >= redg - 1 && dirx>0)){
            dirx *= -1;
            sign *= -1;
        }
        col += sign;

        if(col <= ledg + 1 || col >= redg - 1)
            if(delay > 200){
                delay /= 2;
                set_ticker(delay);
            }
        
    }
}

void posy(){
    int sign;

    sign = diry>0? 1 : -1;
    for(int i=0; i<(diry*sign) ; i++){
        if ((row <= trow + 1 && diry<0) || (row >= brow - 1 && diry>0)){
            diry *= -1;
            sign *= -1;
        }
        row += sign;

        if (row<= trow + 1 || row >= brow - 1){
            if(delay <10000){
                delay *= 2;
                set_ticker(delay);
            }
        }
    }
}

void move_ball(int signum){
    move(row, col);
    addstr(BLANK);
    
    posx();
    posy();
    move(row, col);
    addstr(ball);

    refresh();
}

void start_pingpong(){
    int c;
    void move_ball(int);

    initscr();
    crmode();
    noecho();
    clear();

    for(int i = trow; i <= brow; i++)
        for(int j = ledg; j<=redg; j++)
            if(i==trow || i == brow || j == ledg || j == redg){
                move(i,j);
                addstr("*");
            }
    while( trow>=row || brow<=row || ledg>=col || redg<=col ){
        posx();
        posy();
    }
    move(row, col);
    addstr(ball);

    set_ticker(delay);

    while(true){
        c = getch();
        if(c == 'Q') break;
        if(c == 'q' && dirx>=2) dirx/=2;
        if(c == 'w' && diry>=2) diry/=2;
        if(c == 'e' && dirx<=100) dirx*=2;
        if(c == 'r' && diry<=100) diry*=2;
    }
    endwin();
}

int main(int argc, char* argv[]){
    
    signal(SIGQUIT, SIG_IGN);
    signal(SIGALRM, move_ball);

    ball = argv[1];
    col = atoi(argv[2]);
    row = atoi(argv[3]);
    trow = atoi(argv[4]);
    brow = atoi(argv[5]);
    ledg = atoi(argv[6]);
    redg = atoi(argv[7]);

    dirx = 1;
    diry = 1;
    delay = 1000;

    start_pingpong();
}

