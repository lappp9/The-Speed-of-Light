#include "particle.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct timespec delay = {0, 1000};

track *NewTrack(int maxx){
	track *t = (track *) malloc(sizeof(track));
	t->clock = clock();
	t->head = NULL;
	t->tail = NULL;
	
}

void UpdateWalls(track *t);
void DrawSegments(track *t);