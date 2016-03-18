#ifndef MAP_H
#define MAP_H 

#include <stdlib.h> 
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include "global.h"

using namespace std;

struct Tile {
	Coords location;
	short int type; //-2 wall -1 bomb 0 empty 1-8 near bomb
	short int flag; //-1 untriggerable 0 close 1 open 2 disarm/flagged/bridge 3 bridge/qustion mark
	ALLEGRO_COLOR color;
};

extern short int sizeX, sizeY, bombsCount, currentBombsCount, openTiles;
extern Tile *map;
extern short int *bombsPos, playerPos;  

void setLevel(int x, int y, int b);
bool setBombsPos();
bool setPlayerPos();
bool openTile(Coords location); //return false if bomb explodes
int getTileFromLocation(Coords location);
Coords getLocationFromTile(int tileNumber);
bool allBombsFlagged();
bool allTilesOpen();
bool generateMap();
bool saveMap();
bool loadMap();
void destroyMap();

#endif


