#include <ncurses.h>				/* ncurses.h includes stdio.h */  
#include <string.h> 
#include "particle.h"
#include <time.h>
 
int playMenu(int maxy, int maxx);
int getDuration(FILE* fp);
int stillAlive(trackSegment* s, ship* p);
int duration, direction = 1;

int main()
{
  //make all the variables and initialize a bunch of crap

	int startx, starty, x, y, ch, maxy, maxx,i;
	FILE* fp=fopen("/dev/urandom","r");
  start:
	initscr();						/* start the curses mode */
	cbreak();						/* Line buffering disabled. pass on everything */
	noecho();
	keypad(stdscr, TRUE);
	start_color();			/* Start the color functionality */
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	curs_set(0);
	
	//get the beginning and maximum y coordinates
	getbegyx(stdscr, starty, startx);
	getmaxyx(stdscr, maxy,maxx);
	
	//start off with menu screen
	playMenu(maxy, maxx);
	attron(COLOR_PAIR(1) | A_BOLD);
	
	//initialize the track list
	ship* s = NewShip(maxy-2, (maxx-strlen("<=>"))/2);
	track *t = NewTrack(maxx, maxy);

	//start them with a ship in the middle of the screen
	attron(COLOR_PAIR(3) | A_BOLD);
	drawShip(s);
	attroff(COLOR_PAIR(3));
	
	refresh();	
	
	//THIS IS WHERE THE GAME BEGINS
	while(ch != 'q'){
		//this loop waits for user input to move the ship, if this were
		//the only loop the game would hang until you moved

		getyx(stdscr, y, x);		/* get the current cursor position */
		halfdelay(1);
		if((ch = getch()) != ERR){ 
			switch(ch)
			{ 
				case KEY_LEFT:
				//we need to put a blank word where the ship used to be and redraw it where it's going
				//later i'll replace it with one clear() on each loop and then redraw the whole screen
					if(x > 5){
						moveShipLeft(s);
					}
					break;
				case KEY_RIGHT:
					if(x < maxx-strlen("<=>")){
						moveShipRight(s);
	        }
	        break;
	    }

		}
		clear();
		//get the current players position
		getyx(stdscr, y, x);		/* get the current curser position */
	
		//duration is the number of times i want it to move a direction
		//direction is left or right and switches to the opposite after the track has moved
		//a certain direction for the extent of duration

		duration = getDuration(fp);
	   	
		//mvprintw(0,0,"The time is %d", ((clock()/10000)));
		attron(COLOR_PAIR(1));
		int score = updateTrack(t, direction);
		drawTrack(t);
		attroff(COLOR_PAIR(1));
			
		//move the cursor back so the ship draws correctly
		move(y,x);
		//redraw the ship
		attron(COLOR_PAIR(3));
		drawShip(s);
		attroff(COLOR_PAIR(3));
		if(!stillAlive(t->head, s)){
		  int replay = gameOver(maxy, maxx, score);
		  //mvprintw(0,0,"You lost!");
		  /*if(replay ==1 )
		    refresh();
		    goto start;
		  break;*/
		}
	}
  
  
  refresh();
  attroff(COLOR_PAIR(1));
  free(t);
	free(s);
	endwin();
	return 0;
}

int stillAlive(trackSegment* s, ship* p){
  //if s->leftWing, s->rightWing, or s->center have the same x value as head->left or head->right
    //then return 0 so they go to the death screen
  if(p->leftWing[1] == s->left->c[1] || p->center[1]== s->left->c[1]  || p->rightWing[1]== s->left->c[1]) 
    return 0;
  if(p->leftWing[1] == s->right->c[1] || p->center[1]== s->right->c[1]  || p->rightWing[1]== s->right->c[1])
    return 0;
  return 1;
}

int gameOver(int maxy, int maxx, int score){
  init_pair(2, COLOR_RED, COLOR_WHITE);
  attron(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
	WINDOW *menu_win;
	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	
	int startx = (80 - maxx) / 2;
	int starty = (24 - maxy) / 2;
	menu_win = newwin(maxy, maxx, starty, startx);
	keypad(menu_win, TRUE);
	mvprintw(maxy/2, (maxx-strlen("GAME OVER!"))/2, "GAME OVER!");
	attroff(COLOR_PAIR(2) | A_STANDOUT);
	mvprintw(0,0,"Score: %d!", score);
	mvaddstr(1,0,"Press q to exit");
	char done;
	while((done=getch()) != 'q');/*{
	  if(done == 'r'){
	    endwin();
	    clear();
	    return 1;
	  }
	  if(done == 'q')
	    break;
	}*/
  
	clear();
	endwin();
	exit(0);
}

//display the opening question
int playMenu(int maxy, int maxx){
  init_pair(2, COLOR_RED, COLOR_WHITE);
  attron(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
	WINDOW *menu_win;
	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	
	int startx = (80 - maxx) / 2;
	int starty = (24 - maxy) / 2;
	menu_win = newwin(maxy, maxx, starty, startx);
	keypad(menu_win, TRUE);
	mvprintw(maxy/2, (maxx-strlen("Ready to Play?"))/2, "Ready to play?");
	attroff(COLOR_PAIR(2) | A_STANDOUT);
	getch();
	endwin();
	clear();
	return(0);
}

int getDuration(FILE* fp){

  if(duration){
    duration= duration -1;
    return(duration);
  }
	else{
    //whatever random started as, is how many times it should go to the right or left
    duration = getRandom(fp);
    direction = 1-direction;
    return(duration);
  }
}

int getRandom(FILE* fp){
	int max = 10;
	//have a counter count down from one-hundred so after every one-hundred calls to the function max is decreased by one
	int num;
	if (fp==NULL){
		printf("effed");
		exit(1);	
	}
	
	num = fgetc(fp);
	srand(num);

	return(rand()%max+1); 

	
}


