#ifndef PARTICLE_H
#define PARTICLE_H

#include <time.h>
#include <stdlib.h>

int getRandom(FILE* fp);

typedef struct particle {
	float c[2];  // the y and x coordinates of that particle
	int color;
	int ch;
	struct particle *target;
	struct particle *next;
	struct particle *prev;
} particle;

typedef struct trackSegment{
	struct particle *left;
	struct particle *right;
	int distanceApart;
	struct trackSegment *prev;
	struct trackSegment *next;
} trackSegment;

typedef struct track{
	clock_t clock;
	int maxx;
	int maxy;
	struct trackSegment *head;
	struct trackSegment *tail;
} track;

track *NewTrack(int maxx, int maxy);
trackSegment *NewSegment(int space, track* t, int maxx, int maxy, int random);
particle *NewParticle(int color, int ch, int y, int x);
void UpdateWalls(track *t);
void DrawSegment(track *t);

//struct timespec delay = {0, 1000};

track *NewTrack(int maxx, int maxy){
	FILE* fp=fopen("/dev/urandom","r");
	int i, random;
	track *t = (track *) malloc(sizeof(track));
	t->clock = clock();
	t->maxx = maxx;
	t->maxy = maxy;
	t->tail = NULL;
	t->head = NewSegment(25, t, maxx, maxy, random);
	for(i = 0; i < maxy; i++){	
		random = getRandom(fp);
		trackSegment* curr = NewSegment(25, t, maxx, maxy, random);
	}
	return t;
}

trackSegment *NewSegment(int space, track* t, int maxx, int maxy, int random){
	static int currY;
	//static int currX;
	trackSegment* s = malloc(sizeof(trackSegment));
	s->distanceApart = space;
	/*if(t->tail == NULL){
		t->tail = s;
		t->head->next = s;
		t->tail->prev = t->head;
	}
	else{
		trackSegment* temp = t->tail;
		t->tail = s;
		temp->next = t->tail;
		
	}*/
	currY++;
	
	//next is to actually save these particles in the linked list so they can be updated and also
	//generate a random number to add to the x value so it looks like it's swaying back and forth
	//
	//now it's generating a random move to the left or right but i only want it to move by one
	//but i want it to move over one space a random number of times
	//whatever random returns is the number of times I want the loop to run and draw things in a certain direction
	int j;
	s->left = NewParticle(2, '>', currY, (maxx-strlen("<=>"))/2-20-1);
	s->right = NewParticle(2, '<', currY, (maxx-strlen("<=>"))/2+5-1   );
	
	if(random >=5){
		for(j = 0; j<random; j++){
			mvaddch(s->left->c[0]-j, s->left->c[1], '>');
			mvaddch(s->right->c[0]-j, s->right->c[1], '<');
		}
	}
	else if(random<5){
		for(j = 0; j<random; j++){
			mvaddch(s->left->c[0]+j, s->left->c[1], '>');
			mvaddch(s->right->c[0]+j, s->right->c[1], '<');
		}
	}
	refresh();
	
	return s;
}

particle* NewParticle(int color, int ch, int y, int x){
	particle *p = (particle *) malloc(sizeof(particle));
	p->c[0] = y;  // the y and x coordinates of that particle
	p->c[1] = x;
	p-> color = color;
	p-> ch = ch;
	return p;
}
void DrawSegment(track *t){
	trackSegment *s = (trackSegment *) malloc(sizeof(trackSegment));
	//s->left = 
}

void UpdateWalls(track *t){
  
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
	
	return(rand()%max);
	
}
#endif