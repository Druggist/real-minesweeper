#ifndef MAP_H
#define MAP_H 

#include <cstdlib> 
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <iostream>
#include <fstream>
#include <string>
#include "global.h"

using namespace std;

struct Tile {
	Coords location;
	short int type; //-3 water -2 wall -1 bomb 0 empty 1-8 near bomb
	short int flag; //-1 untriggerable 0 close 1 open 2 disarm/flagged/bridge 3 bridge/qustion mark
	ALLEGRO_COLOR color;
};

extern short int sizeX, sizeY, bombsCount, flaggedBombs, openTiles;
extern Tile *map;
extern short int *bombsPos, playerPos;  

void setLevel(int x, int y, int b);
bool setBombsPos();
bool setPlayerPos();
bool openTile(Coords location); //return false if bomb explodes
void openAll();
int getTileFromLocation(Coords location);
Coords getLocationFromTile(int tileNumber);
bool allBombsFlagged();
bool allTilesOpen();
bool win();
bool generateMap();
bool saveMap(ALLEGRO_DISPLAY *display);
bool loadMap(ALLEGRO_DISPLAY *display, string path);
void toggleTileFlag(Coords location, short int flag);
void destroyMap();

#endif


