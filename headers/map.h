#ifndef MAP_H
#define MAP_H 

#include <stdlib.h> 
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

using namespace std;

struct Coords{
	int x;
	int y;
};
struct Tile {
	Coords location;
	short int type; //-1 bomb 0 empty 1-8 near bomb
	short int flag; //0 close 1 open 2 disarm/flagged/bridge 3 bridge/qustion mark
	ALLEGRO_COLOR color;
};

extern short int sizeX, sizeY, bombsCount, pixels, gap;
extern int displayWidth;
extern Tile *map;
extern short int *bombsPos;  

void setLevel(int x, int y, int b, int p, int g, int d);
bool chooseBombsPos();
bool openTile(Coords location); //return false if bomb explodes
int getTileFromLocation(Coords location);
bool generateMap();
void drawMap();
void drawTile(int drawTile);
bool saveMap();
bool loadMap();
void destroy();

#endif


