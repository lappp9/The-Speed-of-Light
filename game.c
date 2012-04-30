#include <ncurses.h>				/* ncurses.h includes stdio.h */  
#include <string.h> 
#include "particle.h"
#include <time.h>

#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include "helper.h"           /*  our own helper functions  */

#include <stdlib.h>
#include <stdio.h>
#include <libc.h>
#include <pthread.h>

#define ECHO_PORT          (2002)
#define MAX_LINE           (1000)

int playMenu(int maxy, int maxx);
int getDuration(FILE* fp);
int stillAlive(trackSegment* s, ship* p);
int getRandom(FILE* fp);
int gameOver(int maxy, int maxx, int score, int winner);
int duration, direction = 1;
void* serve(void* threadid);

pthread_t server, printer;
int       list_s;                /*  listening socket          */
int       conn_s;                /*  connection socket         */
short int port;                  /*  port number               */
struct    sockaddr_in servaddr;  /*  socket address structure  */
char      buffer[MAX_LINE];      /*  character buffer          */
char     *endptr;                /*  for strtol()              */
char *trimwhitespace(char *str);

void *serve(void *threadid)
{	
	int argc = 1;
	char* argv[2];
	argv[0] = "a.out";
 	if ( argc == 2 ) {
		port = strtol(argv[1], &endptr, 0);
		if ( *endptr ) {
	    	//fprintf(stderr, "ECHOSERV: Invalid port number.\n");
	    	exit(EXIT_FAILURE);
		}
    }
    else if ( argc < 2 ) {
		port = ECHO_PORT;
    }
    else {
		//fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
		exit(EXIT_FAILURE);
    }
    //  Create the listening socket  

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		//fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
    }


    //  Set all bytes in socket address structure to
     //   zero, and fill in the relevant data members

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    //  Bind our socket addresss to the 
	//listening socket, and call listen()  

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
		//fprintf(stderr, "ECHOSERV: Error calling bind()\n");
		exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
		//fprintf(stderr, "ECHOSERV: Error calling listen()\n");
		exit(EXIT_FAILURE);
    }

  // Enter an infinite loop to respond
    //    to client requests and echo input  */

    while ( 1 ) {

		//  Wait for a connection, then accept() it  

		if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
	    	//fprintf(stderr, "ECHOSERV: Error calling accept()\n");
	    	exit(EXIT_FAILURE);
		}


		//  Retrieve an input line from the connected socket
	    //then simply write it back to the same socket.     

		Readline(conn_s, buffer, MAX_LINE-1);
		Writeline(conn_s, buffer, strlen(buffer));

		//  Close the connected socket  
		if ( close(conn_s) < 0 ) {
	    	//fprintf(stderr, "ECHOSERV: Error calling close()\n");
	    	exit(EXIT_FAILURE);
		}
		//pthread_exit(0);
		
    }
}

int main( int argc, char** argv)
{

	//spawn server thread to listen to opponents status
	long targs;
    int rc, sp;
	rc = pthread_create(&server, NULL, serve, (void *)targs);
	//pthread_detach(server);
	
	if (rc){
		printf("ERROR; return code from pthread_create() is %d\n", rc);
	    exit(-1);
	}
	
	//sp = pthread_create(&printer, NULL, printNum, (void *)targs);
	
	//make all the variables and initialize a bunch of crap
	
	//sleep(5);
	//use nanosleep instead
	int startx, starty, x, y, ch, maxy, maxx,i;
	int won = 0;
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

	//start them with a ship in the middle of the screen
	attron(COLOR_PAIR(3) | A_BOLD);
	drawShip(s);
	attroff(COLOR_PAIR(3));
	
	refresh();	
	
	//THIS IS WHERE THE GAME BEGINS
	while(ch != 'q'){
		//move the cursor back to the player before any move is made
		move(maxy-2, s->rightWing[1]+1);		
		//this loop waits for user input to move the ship, if this were
		//the only loop the game would hang until you moved

		getyx(stdscr, y, x);		//get the current cursor position 
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
		getyx(stdscr, y, x);		// get the current curser position 	
		//duration is the number of times i want it to move a direction
		//direction is left or right and switches to the opposite after the track has moved
		//a certain direction for the extent of duration

		duration = getDuration(fp);
		
		char* winner = trimwhitespace(buffer); 
		attron(COLOR_PAIR(1));
		if(strcmp(winner ,"won") == 0){
			mvprintw(2,0,"Your opponent died!");
			won = 1;
		}	
		else{
			mvprintw(2,0,"Your opponent is currently alive!");
			won =0;
		}
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
			if(won == 0){
				won = 2;
			}
		 	int replay = gameOver(maxy, maxx, score, won);
			break;
		}
		
	}
  
  	refresh();
  	attroff(COLOR_PAIR(1));
  	free(t);
	free(s);
	endwin();
	return 0;
}



int gameOver(int maxy, int maxx, int score, int winner){
  	init_pair(4, COLOR_RED, COLOR_WHITE);
	init_pair(3, COLOR_BLUE, COLOR_WHITE);
	WINDOW *menu_win;
	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	
	int startx = (80 - maxx) / 2;
	int starty = (24 - maxy) / 2;
	menu_win = newwin(maxy, maxx, starty, startx);
	keypad(menu_win, TRUE);
	
	if(winner == 2){
		attron(COLOR_PAIR(4) | A_BOLD | A_STANDOUT);
		mvprintw(maxy/2, (maxx-strlen("GAME OVER!"))/2, "GAME OVER!");
		mvprintw((maxy/2)+2, ((maxx-strlen("--You Lose--"))/2), "--You Lose--");
		attroff(COLOR_PAIR(4) | A_STANDOUT);
	}
	else if(winner == 1){
		attron(COLOR_PAIR(3) | A_BOLD | A_STANDOUT);
		mvprintw(maxy/2, (maxx-strlen("CONGRATULATIONS!"))/2, "CONGRATULATIONS!");
		mvprintw((maxy/2)+2, ((maxx-strlen("--You Win!--"))/2), "--You Win!--");
		attroff(COLOR_PAIR(3) | A_STANDOUT);
	}
	mvprintw(0,0,"Score: %d!", score);
	mvaddstr(1,0,"Press q to exit");
	char done;
	while((done=getch()) != 'q');
  	
	clear();
	endwin();
	return 1;
}

//display the opening question
int playMenu(int maxy, int maxx){
  init_pair(2, COLOR_GREEN, COLOR_WHITE);
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

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}