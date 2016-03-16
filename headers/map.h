#include <allegro5/allegro.h>

int sizeX, sizeY;
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

void setSize(int x, int y);
bool generateMap();
bool openTile(Coords location); //return false if bomb explodes
bool checkBridge(Coords location);



