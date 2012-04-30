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
int stillAlive(trackSegment* s, ship* p);
void moveShipLeft(ship* p);
void moveShipRight(ship* p);


#endif