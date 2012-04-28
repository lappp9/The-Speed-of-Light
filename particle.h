#ifndef PARTICLE_H
#define PARTICLE_H

#include <time.h>
#include <stdlib.h>

int getRandom(FILE* fp);

typedef struct ship{
	int leftWing[2];
	int rightWing[2];
	int center[2];
} ship;

typedef struct particle {
	float c[2];  // the y and x coordinates of that particle
	struct particle *next;
	struct particle *prev;
} particle;

typedef struct trackSegment{
	struct particle *left;
	struct particle *right;
	int distanceApart;
	struct trackSegment *next;
} trackSegment;

typedef struct track{
	int maxx;
	int maxy;
	struct trackSegment *head;
	struct trackSegment *tail;
} track;

track *NewTrack(int maxx, int maxy);
trackSegment *NewSegment(int space, track* t, int maxx, int maxy);
particle *NewParticle(int y, int x);
ship* NewShip(int y, int x);
void UpdateWalls(track *t);
void DrawSegment(track *t);
trackSegment* addSegment(track* t);
void drawShip(ship* p);
void updateTrack(track* t, int duration);
void drawTrack(track* t);
void drawSegment(trackSegment* s);

//struct timespec delay = {0, 1000};

void updateTrack(track* t, int duration){
	
	trackSegment* temp;
	mvprintw(0,0,"sup");	
	temp = t->head;
	mvprintw(0,0,"%d",temp->left->c[0]);	
	//y for any segment will never change, but x will shift to the segment above it
	for(temp = t->head; temp->next; temp = temp->next){
		temp->left->c[1] = temp->next->left->c[1];
		temp->right->c[1] = temp->next->right->c[1];
		//mvprintw(0,0,"sup");	
	}
	//after the loop, temp should point to the tail
	//this will have it's x shift one to the left or right based on the random number
	
	
}

void drawTrack(track* t){
	trackSegment* temp;
	
	for(temp = t->head; temp->next; temp = temp->next){
		drawSegment(temp);
	}
}

void drawSegment(trackSegment* s){
	mvaddch(s->left->c[0], s->left->c[1], '>');
	mvaddch(s->right->c[0], s->right->c[1], '<');
}

void moveShipLeft(ship* p){
	p->leftWing[1] -= 3;
	p->center[1] -= 3;
	p->rightWing[1] -= 3;
}

void moveShipRight(ship* p){
	p->leftWing[1] += 3;
	p->center[1] += 3;
	p->rightWing[1] += 3;
}

void drawShip(ship* s){
	mvaddch(s->leftWing[0],s->leftWing[1], '<'); 
	mvaddch(s->center[0],s->center[1], '='); 
	mvaddch(s->rightWing[0],s->rightWing[1], '>'); 	
}

ship *NewShip(int y, int x){
	ship* p = (ship *) malloc(sizeof(ship));
	p->leftWing[0] = y;
	p->rightWing[0]=y;
	p->center[0]=y;
	
	p->leftWing[1]=x;
	p->center[1]=x+1;
	p->rightWing[1]=x+2;
	return p;
}

//this is where the list of segments is created
//returns t which is an object whose head points to the first segment of the track
track *NewTrack(int maxx, int maxy){
	//open this file so we can get a stream of random numbers to be used for the motion
	int i, random;
	
	track *t = (track *) malloc(sizeof(track));
	t->maxx = maxx;
	t->maxy = maxy;
	t->tail = NULL;
	for(i = 0; i < maxy; i++){	
		t->head = NewSegment(10, t, maxx, maxy);
	}
	return t;
}

//this runs the first time and creates a screen full of track segments
trackSegment *NewSegment(int space, track* t, int maxx, int maxy){
	//currY keeps track of where on the screen to draw the next segment
	static int currY, offset;
	int baseX = (maxx-strlen("<=>"))/2;
	currY++;
	offset++;
	
	//new tracksegment
	trackSegment* s = malloc(sizeof(trackSegment));
	s->distanceApart = space;
	
	//add it to the tail of the track
	t->tail = s;
	s->next = NULL;
	
	//offset will be how much you move the pair to the left or right
	//if it's subtracted then it's left, added means to the right
	s->left = NewParticle(currY, baseX-space);
	s->right = NewParticle(currY, baseX+space);
	
	
	
	//drawSegment(s);
	mvaddch(s->left->c[0], s->left->c[1]-offset, '>');
	mvaddch(s->right->c[0], s->right->c[1]+offset, '<');

	refresh();
	
	return s;

}

particle* NewParticle(int y, int x){
	particle *p = (particle *) malloc(sizeof(particle));
	p->c[0] = y;  // the y and x coordinates of that particle
	p->c[1] = x;
	return p;
}


int getRandom(FILE* fp){
	static int max = 10;
	static int counter;
	//have a counter count down from one-hundred so after every one-hundred calls to the function max is decreased by one
	int num;
	if (fp==NULL){
		printf("effed");
		exit(1);	
	}
	
	num = fgetc(fp);
	srand(num);
	num = rand()%max; 
	
	return(num);
	
}
#endif