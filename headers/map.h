#ifndef MAP_H
#define MAP_H 

#include <cstdlib> 
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <fstream>
#include <string>
#include "global.h"

using namespace std;

struct Tile {
	Coords location;
	short int type; //-5 random(bomb/normal) -4 void -3 water -2 wall -1 bomb 0 empty 1-8 near bomb
	short int flag; //-1 untriggerable 0 close 1 open 2 disarm/flagged/bridge 3 bridge/qustion mark
	ALLEGRO_COLOR color;
};
 
struct Colors { 
	ALLEGRO_COLOR close;
	ALLEGRO_COLOR flag;
	ALLEGRO_COLOR questionMark;
	ALLEGRO_FONT *questionMarkFont;  
	ALLEGRO_COLOR questionMarkFontColor;
	ALLEGRO_COLOR bomb;
	ALLEGRO_COLOR spawner;
	ALLEGRO_COLOR freeSpace;
	ALLEGRO_COLOR freeSpaceEditor;
	ALLEGRO_COLOR wall;
	ALLEGRO_COLOR water;
	ALLEGRO_COLOR empty;
	ALLEGRO_COLOR nearBomb;
};

extern short int sizeX, sizeY, bombsCount, flaggedBombs, openTiles;
extern Tile *map;
extern Colors mapColors;
extern short int *bombsPos, playerPos;  

void setLevel(int x, int y, int b);
bool openTile(Coords location); //return false if bomb explodes
void openAll();
void closeAll();
int getTileFromLocation(Coords location);
Coords getLocationFromTile(int tileNumber);
bool allBombsFlagged();
bool allTilesOpen();
bool win();
bool generateMap();
bool saveMap(ALLEGRO_DISPLAY *display);
bool loadMap(ALLEGRO_DISPLAY *display, string path);
int toggleTileFlag(Coords location, short int flag);
void destroyMap();
bool createMap();
int getNormalTilesCount();
int getPlacedBombsCount();
int getRandomTilesCount();
Coords generateSpawnerLocation();
bool generateBombs(int randomElements, int bombsQuantity);
int getFirstRandomTile();
bool fillRandomTiles();
void setUntriggerableColors();
bool canMove(Coords location);
//editor
void setEditorColors();
int checkNeighbours(Coords location);
void setEmpty(Coords location); //1
void setBomb(Coords location); //2
void removeBomb(Coords location); 
bool setSpawner(Coords location); //3
void setWall(Coords location); //4
void setWater(Coords location); //5
void setFreeSpace(Coords location); //6
void setRandom(Coords location); //7
#endif


