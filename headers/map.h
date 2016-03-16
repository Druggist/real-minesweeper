#ifndef MAP_H
#define MAP_H 

#include <stdlib.h> 
#include <allegro5/allegro.h>
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

extern short int sizeX, sizeY, pixels, bombsCount;
extern Tile *map;
extern short int *bombsPos;  

void setLevel(int x, int y, int p, int b);
bool generateMap();
bool chooseBombsPos();
bool openTile(Coords location); //return false if bomb explodes
short int getTileFromLocation(Coords location);
void draw();
void destroy();

#endif


