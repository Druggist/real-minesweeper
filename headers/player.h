#ifndef PLAYER_H
#define PLAYER_H 

#include <allegro5/allegro.h>
#include "global.h"

struct Player{
	Coords location;
	ALLEGRO_COLOR color;
	ALLEGRO_COLOR equipmentColor;
	short int flagsEquipped;	
};

extern short int sizeX, sizeY;
extern int displayWidth;
extern Player player;

void getInput();
void spawnPlayer(Coords location, short int flagsEquipped);
void move(Coords nextLocation);
void putFlag();
void takeFlag();
bool hasFlags();

#endif