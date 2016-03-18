#ifndef PLAYER_H
#define PLAYER_H 

#include <allegro5/allegro.h>
#include "global.h"

struct Player{
	Coords location;
	ALLEGRO_COLOR color;
	short int flagsEquipped;	
};

extern short int sizeX, sizeY;
extern int displayWidth;
extern Player player;

void getInput();
void spawnPlayer(Coords location, short int flagsEquipped);
Coords moveLeft();
Coords moveRight();
Coords moveUp();
Coords moveDown();


#endif