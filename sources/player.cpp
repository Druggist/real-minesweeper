#include "../headers/player.h"

ALLEGRO_KEYBOARD_STATE keyboard;
Player player;

void spawnPlayer(Coords location, short int flagsEquipped){
	player.location = location;
	player.color = al_map_rgb(205, 220, 57);
	return;
}

Coords moveLeft(){
	if(player.location.x > 0) player.location.x--;
	return player.location;
}

Coords moveRight(){
	if(player.location.x < sizeX - 1 ) player.location.x++;
	return player.location;
}

Coords moveUp(){
	if(player.location.y > 0) player.location.y--;
	return player.location;
}

Coords moveDown(){
	if(player.location.y < sizeY - 1 ) player.location.y++;
	return player.location;
}