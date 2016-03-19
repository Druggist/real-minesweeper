#include "../headers/map.h"

short int sizeX, sizeY, bombsCount, flaggedBombs, openTiles;
int displayWidth;
Tile *map;
short int *bombsPos, playerPos;  

void setLevel(int x, int y, int b){
	sizeX = x;
	sizeY = y;
	bombsCount = b;
	flaggedBombs = 0;
	openTiles = 1;
	return;
}

bool generateMap(){
	destroyMap();
	int size = sizeX*sizeY;
	if(size > 0){
		map = new Tile[size];
		if (!setBombsPos()) return false;
		int currentBombsCount = 0;
		for(int i = 0; i < size; i++){
			map[i].location.x = i % sizeX;
			map[i].location.y = i / sizeX;
			map[i].flag = 0;
			map[i].type = 0;
			map[i].color = al_map_rgb(230, 81, 0);
		}
			
		for(int i = 0; i < size; i++){
			if(i == bombsPos[currentBombsCount]){
				map[i].type = -1;

				//left side 
				if(i % sizeX > 0){
					if(map[i - 1].type != -1)	map[i - 1].type++;
					if(i / sizeX > 0 && map[i - sizeX - 1].type != -1) map[i - sizeX - 1].type++;
					if(i / sizeX < sizeY - 1 && map[i + sizeX - 1].type != -1) map[i + sizeX - 1].type++;
				}

				//center
				if(i / sizeX > 0 && map[i - sizeX].type != -1) map[i - sizeX].type++;
				if(i / sizeX < sizeY - 1 && map[i + sizeX].type != -1) map[i + sizeX].type++;

				//right side
				if(i % sizeX < sizeX - 1){
					if(map[i + 1].type != -1)	map[i + 1].type++;
					if(i / sizeX > 0 && map[i - sizeX + 1].type != -1) map[i - sizeX + 1].type++;
					if(i / sizeX < sizeY - 1 && map[i + size + 1].type != -1) map[i + sizeX + 1].type++;
				}
				currentBombsCount++;
			}

		}
		if(!setPlayerPos()) return false; 
	} else return false;
	return true;
}

bool setBombsPos(){
	if(bombsCount > 0 && bombsCount < sizeX * sizeY){
		bombsPos = new short int[bombsCount];
		for(int i = 0; i < bombsCount; i++){
			bombsPos[i] = rand() % (sizeX * sizeY);
			if (i > 0){
				bool isEqual = false;
				for(int j = 0; j < i; j++){
					if(bombsPos[i] == bombsPos[j]){
						i--;
						isEqual = true;
					}
				}

				if(!isEqual){
					for(int j = 1; j <= i; j++){
						if (bombsPos[i - j + 1] < bombsPos[i - j]){
							int temp = bombsPos[i - j + 1];
							bombsPos[i - j + 1] = bombsPos[i - j];
							bombsPos[i - j] = temp;
						}else break;
					} 
				}
			}
		}
	} else return false;
	return true;
}

bool setPlayerPos(){
	playerPos = rand() % (sizeX * sizeY);
	if(bombsCount == sizeX * sizeY) return false;
	while(map[playerPos].type == -1) playerPos = rand() % (sizeX * sizeY);
	map[playerPos].color = al_map_rgb(175, 180, 43);
	map[playerPos].flag = 1;
	return true;
}

bool openTile(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(map[tileNumber].flag == 0){
		map[tileNumber].flag = 1;
		switch(map[tileNumber].type){
			case -1:
			map[tileNumber].color = al_map_rgb(33, 33, 33);
			return false;
			break;		
	
			case 0:
			map[tileNumber].color = al_map_rgb(255, 204, 128);
			Coords tempLocation;
			//left
			if(location.x > 0) {
				tempLocation.x = location.x - 1;
				tempLocation.y = location.y;
				openTile(tempLocation);	
				if(location.y > 0){
					tempLocation.x = location.x - 1;
					tempLocation.y = location.y - 1;
					openTile(tempLocation);	
				}
				if(location.y < sizeY - 1){
					tempLocation.x = location.x - 1;
					tempLocation.y = location.y + 1;
					openTile(tempLocation);	
				}
			}
			//right
			if(location.x < sizeX - 1) {
				tempLocation.x = location.x + 1;
				tempLocation.y = location.y;
				openTile(tempLocation);
				if(location.y > 0){
					tempLocation.x = location.x + 1;
					tempLocation.y = location.y - 1;
					openTile(tempLocation);	
				}
				if(location.y < sizeY - 1){
					tempLocation.x = location.x + 1;
					tempLocation.y = location.y + 1;
					openTile(tempLocation);	
				}	
			}
			//top
			if(location.y > 0) {
				tempLocation.x = location.x;
				tempLocation.y = location.y - 1;
				openTile(tempLocation);	
			}
			//bottom
			if(location.y < sizeY - 1) {
				tempLocation.x = location.x;
				tempLocation.y = location.y + 1;
				openTile(tempLocation);	
			}
			break;

			default:
			map[tileNumber].color = al_map_rgb(255, 152, 0);
			break;
		}
		openTiles++;
	}
	return true;
}

int getTileFromLocation(Coords location){
	return location.x + sizeX * location.y;
}

Coords getLocationFromTile(int tileNumber){
	return map[tileNumber].location;
}

bool allTilesOpen(){
	if(openTiles == sizeX * sizeY - bombsCount) return true;
	return false;
}

bool allBombsFlagged(){
	if(flaggedBombs == bombsCount) return true;
	return false;
}

bool win(){
	if(allBombsFlagged() || allTilesOpen()) return true;
	return false;
}

void toggleTileFlag(Coords location, short int flag){
	if(map[getTileFromLocation(location)].flag != -1 && map[getTileFromLocation(location)].flag != 1){
		map[getTileFromLocation(location)].flag = (map[getTileFromLocation(location)].flag != flag)?(flag):(0);
		ALLEGRO_COLOR color;
		switch(map[getTileFromLocation(location)].flag){
			case 2: 
				color = al_map_rgb(183, 28, 28);
				break;

			case 3:
				color = al_map_rgb(93, 64, 55);
				break;

			default:
				color = al_map_rgb(230, 81, 0);
				break;
		}
		map[getTileFromLocation(location)].color = color;
		if(map[getTileFromLocation(location)].type == -1 && flag == 2) flaggedBombs++;
	}
	return;
}

void openAll(){
	for(int i = 0; i < sizeX * sizeY; i++){
		if(map[i].flag != -1) openTile(getLocationFromTile(i));
	}
	return;
}

void destroyMap(){
	if(map != NULL) delete[] map;
	if(bombsPos != NULL) delete[] bombsPos;
	return;
}