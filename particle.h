#ifndef PARTICLE_H
#define PARTICLE_H

#include <time.h>
#include <stdlib.h>


typedef struct ship{
	int leftWing[2];
	int rightWing[2];
	int center[2];
} ship;

typedef struct particle {
	float c[2];  // the y and x coordinates of that particle
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
void drawShip(ship* p);
int updateTrack(track* t, int direction);
void drawTrack(track* t);
void drawSegment(trackSegment* s);

//returns current score
int updateTrack(track* t, int direction){
  static int difficultyCount;
  static int difficulty;
	trackSegment* temp;
	temp = t->head;
  int i;
	//y for any segment will never change, but x will shift to the segment above it
	for(temp = t->head; temp->next; temp = temp->next){
		temp->left->c[1] = temp->next->left->c[1];
		temp->right->c[1] = temp->next->right->c[1];
	}

	//make it harder every 10 ticks


	  //when changing the tail it will be shifted left or right by one depedning on duration being 0 or 1
    if(direction == 1){
    	t->tail->left->c[1] =	t->tail->left->c[1]+1; 
    	t->tail->right->c[1] =	t->tail->right->c[1]+1; 	
    }
    else{
    	t->tail->left->c[1] =	t->tail->left->c[1]-1; 
    	t->tail->right->c[1] =	t->tail->right->c[1]-1;
    }
    
  if((++difficultyCount%100) == 0 && difficulty < 6){
    //t->tail->left->c[1] =	t->tail->left->c[1]+1;
  	  t->tail->right->c[1] = t->tail->right->c[1]-1;
  	  t->tail->left->c[1] = t->tail->left->c[1]+1;
      difficulty++;
	}
  mvprintw(0,0,"Score: %d",(difficultyCount));
  return(difficultyCount);
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
	p->leftWing[1] -= 2;
	p->center[1] -= 2;
	p->rightWing[1] -= 2;
}

void moveShipRight(ship* p){
	p->leftWing[1] += 2;
	p->center[1] += 2;
	p->rightWing[1] += 2;
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
	int i, random;
	
	track *t = (track *) malloc(sizeof(track));
	t->maxx = maxx;
	t->maxy = maxy;
	t->tail = NewSegment(10, t, maxx, maxy);
	drawSegment(t->tail);
	for(i = 0; i < maxy-2; i++){
		t->head = NewSegment(10, t, maxx, maxy);
		//drawSegment(t->head);
	}
	drawSegment(t->head);

	refresh();
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
	s->next = t->head;
	t->head = s;
	
	//offset will be how much you move the pair to the left or right
	//if it's subtracted then it's left, added means to the right
	s->left = NewParticle(currY, baseX-space);
	s->right = NewParticle(currY, baseX+space);
	
	refresh();
	
	return s;

}

particle* NewParticle(int y, int x){
	particle *p = (particle *) malloc(sizeof(particle));
	p->c[0] = y;  // the y and x coordinates of that particle
	p->c[1] = x;
	return p;
}

#endif