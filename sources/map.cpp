#include "../headers/map.h"

short int sizeX, sizeY, bombsCount, pixels, gap;
int displayWidth;
Tile *map;
short int *bombsPos;  

void setLevel(int x, int y, int b, int p, int g, int d){
	sizeX = x;
	sizeY = y;
	bombsCount = b;
	pixels = p;
	gap = g;
	displayWidth = d;
	return;
}

bool generateMap(){
	int size = sizeX*sizeY;
	if(size > 0){
		map = new Tile[size];
		if (!chooseBombsPos()) return false;
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
					if(i / sizeX < sizeY && map[i + sizeX - 1].type != -1) map[i + sizeX - 1].type++;
				}

				//center
				if(i / sizeX > 0 && map[i - sizeX].type != -1) map[i - sizeX].type++;
				if(i / sizeX < sizeY && map[i + sizeX].type != -1) map[i + sizeX].type++;

				//right side
				if(i % sizeX < sizeX - 1){
					if(map[i + 1].type != -1)	map[i + 1].type++;
					if(i / sizeX > 0 && map[i - sizeX + 1].type != -1) map[i - sizeX + 1].type++;
					if(i / sizeX < sizeY && map[i + size + 1].type != -1) map[i + sizeX + 1].type++;
				}
				currentBombsCount++;
			}

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
			drawMap();
			return false;
			break;		
	
			case 0:
			map[tileNumber].color = al_map_rgb(255, 183, 77);
			drawMap();
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
			map[tileNumber].color = al_map_rgb(255, 152, 0);
			drawMap();
			break;
		}
	}
	return true;
}

int getTileFromLocation(Coords location){
	return location.x + sizeX * location.y;
}

void drawMap(){
	al_clear_to_color(al_map_rgb(255, 243, 224));
	for(int i=0; i<sizeX*sizeY; i++) drawTile(i);
	al_flip_display();
}

void drawTile(int tileNumber){
	int horizontalGap, verticalGap, centered;
	centered = (displayWidth > sizeX * pixels + (sizeX - 1) * gap)?((displayWidth - (sizeX * pixels + (sizeX - 1) * gap))/2):(0);
	if(map[tileNumber].location.x == 0) verticalGap = 0;
		else verticalGap = gap;
		if(map[tileNumber].location.y == 0) horizontalGap = 0;
		else horizontalGap = gap;
		int startX = map[tileNumber].location.x * pixels + verticalGap + centered;
		int startY = map[tileNumber].location.y * pixels + horizontalGap;
		int endX = map[tileNumber].location.x * pixels + pixels + centered;
		int endY = map[tileNumber].location.y * pixels + pixels;
		al_draw_filled_rectangle(startX, startY, endX, endY, map[tileNumber].color);
}

void destroy(){
	if(map != NULL) delete[] map;
	if(bombsPos != NULL) delete[] bombsPos;
	return;
}