#include "../headers/player.h"

ALLEGRO_KEYBOARD_STATE keyboard;
Player player;

void spawnPlayer(Coords location, short int flagsEquipped){
	player.location = location;
	player.color = al_map_rgb(205, 220, 57);
	player.equipmentColor = al_map_rgb(175, 180, 43);
	player.flagsEquipped = flagsEquipped;
	return;
}

void move(Coords nextLocation){
	player.location = nextLocation;
	return;
}

void putFlag(){
	player.flagsEquipped--;
}

void takeFlag(){
	player.flagsEquipped++;
}

bool hasFlags(){
	if(player.flagsEquipped > 0) return true;
	return false;
}


