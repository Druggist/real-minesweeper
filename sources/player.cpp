#include "../headers/player.h"

ALLEGRO_KEYBOARD_STATE keyboard;
Player player;

void spawnPlayer(Coords location, short int flagsEquipped){
	player.location = location;
	player.color = al_map_rgb(205, 220, 57);
	return;
}

void move(Coords nextLocation){
	player.location = nextLocation;
	return;
}
