#include <ncurses.h>				/* ncurses.h includes stdio.h */  
#include <string.h> 
 #define WIDTH 30
#define HEIGHT 10 
int playMenu(int maxy, int maxx);

int main()
{
  //make all the variables and initialize a bunch of crap
	int startx, starty, width, height, x, y, ch, maxy, maxx;
	int row,col;					/* to store the number of rows and */
								/* the number of colums of the screen */
	initscr();						/* start the curses mode */
	cbreak();						/* Line buffering disabled. pass on everything */
	noecho();
	keypad(stdscr, TRUE);
	start_color();			/* Start the color functionality */
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	curs_set(0);
	
	//get the beginning and maximum y coordinates
	getbegyx(stdscr, starty, startx);
	getmaxyx(stdscr, maxy,maxx);
	
	//start of with menu screen
	playMenu(maxy, maxx);
	
	
	//print out some info for myself
	mvprintw(starty, startx, "The window begins at y: %d and x: %d in the top left and y: %d and x: %d at the bottom right.", starty, startx, maxy,maxx);
	
	//start them with a ship in the middle of the screen
	mvprintw(maxy-2,(maxx-strlen("<=>"))/2, "<=>"); 
	refresh();
	starty = (LINES - height) / 2;	
	startx = (COLS - width) / 2;	
	attroff(COLOR_PAIR(1));

	
	//THIS IS WHERE THE GAME BEGINS
	while(1){
		while((ch = getch()) != 'q')
    { 
      getyx(stdscr, y, x);		/* get the current curser position */
      switch(ch)
      { 
        case KEY_LEFT:
          //we need to put a blank word where the ship used to be and redraw it where it's going
          //later i'll replace it with one clear() on each loop and then redraw the whole screen
          if(x > 5){
            mvaddstr(y, x-3, "   ");
            //clear();
            mvaddstr(y, x-4, "<=>");
          }
          break;
        case KEY_RIGHT:
          if(x < maxx-strlen("<=>")){
            mvaddstr(y, x-3, "   ");
            //clear();
            mvaddstr(y, x-2, "<=>");
          }
          break;
       }
    }

		attron(COLOR_PAIR(1));
		refresh();
		attroff(COLOR_PAIR(1));
	  endwin();
	  return 0;
	}
}

int playMenu(int maxy, int maxx){
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
	getch();
	endwin();
	clear();
	return(0);
	
}