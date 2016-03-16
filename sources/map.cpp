#include "../headers/map.h"

short int sizeX, sizeY, pixels, bombsCount;
Tile *map;
short int *bombsPos;  

void setLevel(int x, int y, int p, int b){
	sizeX = x;
	sizeY = y;
	pixels = p;
	bombsCount = b;
	return;
}

bool generateMap(){
	int size = sizeX*sizeY;
	if(size > 0){
		map = new Tile[size];
		short int y = -1;
		if (!chooseBombsPos()) return false;
		int currentBombsCount = 0;
		for(int i = 0; i < size; i++){
			if(i % sizeX == 0) y++;
			map[i].location.x = i % sizeX;
			map[i].location.y = y;
			
			if(i == bombsPos[currentBombsCount]){
				map[i].type = -1;

				//left side 
				if(i % sizeX != 0){
					if(map[i - 1].type != -1)	map[i - 1].type++;
					if(y != 0 && map[i - sizeX - 1].type != -1) map[i - sizeX - 1].type++;
					if(y != sizeY && map[i + sizeX - 1].type != -1) map[i + sizeX - 1].type++;
				}

				//center
				if(y != 0 && map[i - sizeX].type != -1) map[i - sizeX].type++;
				if(y != sizeY && map[i + sizeX].type != -1) map[i + sizeX].type++;

				//right side
				if(i % sizeX != sizeX - 1){
					if(map[i + 1].type != -1)	map[i + 1].type++;
					if(y != 0 && map[i - sizeX + 1].type != -1) map[i - sizeX + 1].type++;
					if(y != sizeY && map[i + size + 1].type != -1) map[i + sizeX + 1].type++;
				}
				currentBombsCount++;
			}

			map[i].color = al_map_rgb(158, 158, 158);
		}
	} else return false;
	return true;
}

bool chooseBombsPos(){
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
			map[tileNumber].color = al_map_rgb(189, 189, 189);
			Coords tempLocation;
			//left
			if(location.x % sizeX != 0) {
				tempLocation.x = location.x - 1;
				tempLocation.y = location.y;
				openTile(tempLocation);	
			}
			//right
			if(location.x % sizeX != sizeX - 1) {
				tempLocation.x = location.x + 1;
				tempLocation.y = location.y;
				openTile(tempLocation);	
			}
			//top
			if(location.y != 0) {
				tempLocation.x = location.x;
				tempLocation.y = location.y - 1;
				openTile(tempLocation);	
			}
			//bottom
			if(location.y != sizeY - 1) {
				tempLocation.x = location.x;
				tempLocation.y = location.y + 1;
				openTile(tempLocation);	
			}
			break;

			default:
			map[tileNumber].color = al_map_rgb(224, 224, 224);
			break;
		}
	}
	return true;
}

int getTileFromLocation(Coords location){
	return location.x + sizeX * location.y;
}

void drawMap(){
	int horizontalGap, verticalGap;
	for(int i=0; i<sizeX*sizeY; i++)
	{
		if(map[i].location.x == 0) verticalGap = 0;
		else verticalGap = 5;
		if(map[i].location.y == 0) horizontalGap = 0;
		else horizontalGap = 5;
		//al_draw_filled_rectangle(i, i, i+10, i+10, al_map_rgb(255, 255, 255));
		al_draw_filled_rectangle(map[i].location.x * pixels + verticalGap, map[i].location.y * pixels + horizontalGap, map[i].location.x * pixels + pixels, map[i].location.y * pixels + pixels, map[i].color);
	 	al_flip_display();
	}
}

void destroy(){
	if(map != NULL) delete[] map;
	if(bombsPos != NULL) delete[] bombsPos;
	return;
}