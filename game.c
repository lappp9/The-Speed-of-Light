#include <ncurses.h>				/* ncurses.h includes stdio.h */  
#include <string.h> 
#include "particle.h"
#include <libc.h>
 
int playMenu(int maxy, int maxx);


int main()
{
  //make all the variables and initialize a bunch of crap
	int startx, starty, x, y, ch, maxy, maxx,i,duration, direction;
	FILE* fp=fopen("/dev/urandom","r");
						
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
	//initialize the track list//
	
	//print out some info for myself
	//mvprintw(starty, startx, "The window begins at y: %d and x: %d in the top left and y: %d and x: %d at the bottom right.", starty, startx, maxy,maxx);
	
	//start them with a ship in the middle of the screen
	attron(COLOR_PAIR(3) | A_BOLD);
	drawShip(s);
	attroff(COLOR_PAIR(3));
	
	refresh();	
	
	
	//THIS IS WHERE THE GAME BEGINS
	while(ch != 'q'){
		//this loop waits for user input to move the ship, if this were
		//the only loop the game would hang until you moved
		halfdelay(5);
		getyx(stdscr, y, x);		/* get the current curser position */
		if((ch = getch()) != ERR)
	    { 
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
			//clear();
			
			//get the current players position
			getyx(stdscr, y, x);		/* get the current curser position */
			//redraw the track
			//sleep((1/20));  //use this to make the track redraw more slowly or quickly
			/*if(duration == 0){
				direction = 1-direction;
				duration = getRandom(fp);			
			}
			else{
				//whatever random started as is how many times it should go to the right or left
				duration--;
			}*/
			updateTrack(t, duration);
			drawTrack(t);
			//mvprintw(0,0,"sup");	
			
			//move the cursor back so the ship draws correctly
			move(y,x);
			//redraw the ship
			attron(COLOR_PAIR(3));
			drawShip(s);
 			attroff(COLOR_PAIR(3));
		}
		

		//now redraw the track and check for a collision
		//create a new node, shift all nodes y's by one, delete the last one
		
		
		
		//t->head = NewSegment(10, t, maxx, maxy, random);
    }
	
	refresh();
	attroff(COLOR_PAIR(1));
	free(t);
	free(s);
	endwin();
	return 0;
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



