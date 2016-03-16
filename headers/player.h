#include <allegro5/allegro.h>

ALLEGRO_KEYBOARD_STATE keyboard; 
struct Coords{
	int x;
	int y;
};
struct player{
	Coords location;
	ALLEGRO_COLOR color;	
};

void initialize();
void getInput();
void setLocation();
