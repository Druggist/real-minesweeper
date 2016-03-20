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
	if(bombsCount < sizeX * sizeY){
		if(bombsCount > 0){
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
		}else{
			bombsPos = new short int[1];
			bombsPos[0] = -1;
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

bool saveMap(ALLEGRO_DISPLAY *display){
	if(!al_init_native_dialog_addon()) return false;
	ALLEGRO_FILECHOOSER *filechooser;
	filechooser = al_create_native_file_dialog("~/new.map", "Save map.", "*.map;", ALLEGRO_FILECHOOSER_SAVE);
	al_show_native_file_dialog(display, filechooser);
	if(al_get_native_file_dialog_count(filechooser) > 0){
		fstream file;
		file.open(al_get_native_file_dialog_path(filechooser, 0), ios::out);
		if(!file.good()) return false;
		file <<sizeX <<" " <<sizeY <<" " <<bombsCount <<endl;
		file <<playerPos <<endl;
		for(int i = 0; i < sizeX * sizeY; i++){
			short int flag = (i == playerPos)?(map[i].flag):(0);
			file <<map[i].location.x <<" " <<map[i].location.y <<" " <<map[i].type <<" " <<flag <<endl;
		}
		file.close();
	}
	al_shutdown_native_dialog_addon();
	return true;
}

bool loadMap(ALLEGRO_DISPLAY *display, string path){
	if(!al_init_native_dialog_addon()) return false;
	if(path == ""){
		ALLEGRO_FILECHOOSER *filechooser;
		filechooser = al_create_native_file_dialog("~/", "Load map.", "*.map;", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
		al_show_native_file_dialog(display, filechooser);
		if(al_get_native_file_dialog_count(filechooser) > 0) path = al_get_native_file_dialog_path(filechooser, 0);
		else return false;
	}
	fstream file;
	file.open(path, ios::in);
	if(!file.good()) return false;
	destroyMap();
	string data;
	flaggedBombs = 0;
	openTiles = 1;
	getline(file, data, ' ');
	sizeX = atoi(data.c_str());
	if(sizeX > 100 || sizeX < 2) return false;
	getline(file, data, ' ');
	sizeY = atoi(data.c_str());
	if(sizeY > 100 || sizeY < 2) return false;
	getline(file, data);
	bombsCount = atoi(data.c_str());
	if(bombsCount > sizeX * sizeY - 2 || bombsCount < 1) return false;
	getline(file, data);
	playerPos = atoi(data.c_str());
	if(playerPos >= sizeX * sizeY) return false;
	map = new Tile[sizeX * sizeY];
	for(int i = 0; i < sizeX * sizeY; i++){
		for(int j = 0; j < 4; j++){
			if(j == 3) getline(file, data);
			else getline(file, data, ' ');
			switch(j){
				case 0:
					map[i].location.x = atoi(data.c_str());
					if(map[i].location.x > sizeX - 1 || map[i].location.x < 0) return false;
					break;
				case 1:
					map[i].location.y = atoi(data.c_str());
					if(map[i].location.y > sizeY - 1 || map[i].location.y < 0) return false;
					break;
				case 2:
					map[i].type = atoi(data.c_str());
					if(map[i].type > 8 || map[i].type < -5) return false;
					break;
				case 3:
					map[i].flag = atoi(data.c_str());
					if(map[i].flag > 3 || map[i].flag < -1) return false;
					break;					
			}
		}
		if(i != playerPos) map[i].color = al_map_rgb(230, 81, 0);
		else map[i].color = al_map_rgb(175, 180, 43);
	}
	file.close();
	al_shutdown_native_dialog_addon();
	return true;
}

void destroyMap(){
	if(map != NULL) delete[] map;
	if(bombsPos != NULL) delete[] bombsPos;
	return;
}

int checkNeighbours(Coords location){
	int count = 0;
	Coords neighbourLocation = location;
	if(location.x > 0){
		neighbourLocation.x = location.x - 1;
		if(map[getTileFromLocation(neighbourLocation)].type == -1) count++;
		if(location.y > 0){
			neighbourLocation.y = location.y - 1;
			if(map[getTileFromLocation(neighbourLocation)].type == -1) count++;
		}
		if(location.y < sizeY - 1){
			neighbourLocation.y = location.y + 1;
			if(map[getTileFromLocation(neighbourLocation)].type == -1) count++;
		}
	}
	if(location.y > 0){
		neighbourLocation.x = location.x;
		neighbourLocation.y = location.y - 1;
		if(map[getTileFromLocation(neighbourLocation)].type == -1) count++;
	}
	if(location.y < sizeY - 1){
		neighbourLocation.x = location.x;
		neighbourLocation.y = location.y + 1;
		if(map[getTileFromLocation(neighbourLocation)].type == -1) count++;
	}
	if(location.x < sizeX - 1){
		neighbourLocation.x = location.x + 1;
		neighbourLocation.y = location.y;
		if(map[getTileFromLocation(neighbourLocation)].type == -1) count++;
		if(location.y > 0){
			neighbourLocation.y = location.y - 1;
			if(map[getTileFromLocation(neighbourLocation)].type == -1) count++;
		}
		if(location.y < sizeY - 1){
			neighbourLocation.y = location.y + 1;
			if(map[getTileFromLocation(neighbourLocation)].type == -1) count++;
		}
	}

	return count;
}

//editor
void setEmpty(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
		if(map[tileNumber].type == -1) removeBomb(location);
		map[tileNumber].type = checkNeighbours(location);
		map[tileNumber].flag = 1;
		map[tileNumber].color = (map[tileNumber].type == 0)?(al_map_rgb(255, 204, 128)):(al_map_rgb(255, 152, 0));
	} 
	return;
}

void removeBomb(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
				if(map[tileNumber].type != -1){ 
			//left side 
			if(location.x > 0){
				if(map[tileNumber - 1].type > 0){
					map[tileNumber - 1].type--;
					map[tileNumber - 1].color = (map[tileNumber - 1].type == 0)?(al_map_rgb(255, 204, 128)):(al_map_rgb(255, 152, 0));
				}
				if(location.y > 0 && map[tileNumber - sizeX - 1].type > 0){
					map[tileNumber - sizeX - 1].type--;
					map[tileNumber - sizeX - 1].color = (map[tileNumber - sizeX - 1].type == 0)?(al_map_rgb(255, 204, 128)):(al_map_rgb(255, 152, 0));
				}
				if(location.y < sizeY - 1 && map[tileNumber + sizeX - 1].type > 0) {
					map[tileNumber + sizeX - 1].type--;
					map[tileNumber + sizeX - 1].color = (map[tileNumber + sizeX - 1].type == 0)?(al_map_rgb(255, 204, 128)):(al_map_rgb(255, 152, 0));
				}
			}

			//center
			if(location.y > 0 && map[tileNumber - sizeX].type > 0) {
				map[tileNumber - sizeX].type--;
				map[tileNumber - sizeX].color = (map[tileNumber - sizeX].type == 0)?(al_map_rgb(255, 204, 128)):(al_map_rgb(255, 152, 0));
			}
			if(location.y < sizeY - 1 && map[tileNumber + sizeX].type > 0){
				map[tileNumber + sizeX].type--;
				map[tileNumber + sizeX].color = (map[tileNumber + sizeX].type == 0)?(al_map_rgb(255, 204, 128)):(al_map_rgb(255, 152, 0));
			}

			//right side
			if(location.x < sizeX - 1){
				if(map[tileNumber + 1].type > 0){
					map[tileNumber + 1].type--;
					map[tileNumber + 1].color = (map[tileNumber + 1].type == 0)?(al_map_rgb(255, 204, 128)):(al_map_rgb(255, 152, 0));
				}
				if(location.y > 0 && map[tileNumber - sizeX + 1].type > 0){
					map[tileNumber - sizeX + 1].type--;
					map[tileNumber - sizeX + 1].color = (map[tileNumber - sizeX + 1].type == 0)?(al_map_rgb(255, 204, 128)):(al_map_rgb(255, 152, 0));
				}
				if(location.y < sizeY - 1 && map[tileNumber + sizeX + 1].type > 0){
					map[tileNumber + sizeX + 1].type--;
					map[tileNumber + sizeX + 1].color = (map[tileNumber + sizeX + 1].type == 0)?(al_map_rgb(255, 204, 128)):(al_map_rgb(255, 152, 0));
				}
			}
			bombsCount--;
			map[tileNumber].type = checkNeighbours(location);
			map[tileNumber].flag = 1;
			map[tileNumber].color = (map[tileNumber].type == 0)?(al_map_rgb(255, 204, 128)):(al_map_rgb(255, 152, 0));
		}
	} 
	return;
}

void setBomb(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
		if(map[tileNumber].type != -1){ 
			//left side 
			if(location.x > 0){
				if(map[tileNumber - 1].type > -1){
					map[tileNumber - 1].type++;
					map[tileNumber - 1].color = al_map_rgb(255, 152, 0);
				}
				if(location.y > 0 && map[tileNumber - sizeX - 1].type > -1){
					map[tileNumber - sizeX - 1].type++;
					map[tileNumber - sizeX - 1].color = al_map_rgb(255, 152, 0);
				}
				if(location.y < sizeY - 1 && map[tileNumber + sizeX - 1].type > -1) {
					map[tileNumber + sizeX - 1].type++;
					map[tileNumber + sizeX - 1].color = al_map_rgb(255, 152, 0);
				}
			}

			//center
			if(location.y > 0 && map[tileNumber - sizeX].type > -1) {
				map[tileNumber - sizeX].type++;
				map[tileNumber - sizeX].color = al_map_rgb(255, 152, 0);
			}
			if(location.y < sizeY - 1 && map[tileNumber + sizeX].type > -1){
				map[tileNumber + sizeX].type++;
				map[tileNumber + sizeX].color = al_map_rgb(255, 152, 0);
			}

			//right side
			if(location.x < sizeX - 1){
				if(map[tileNumber + 1].type > -1){
					map[tileNumber + 1].type++;
					map[tileNumber + 1].color = al_map_rgb(255, 152, 0);
				}
				if(location.y > 0 && map[tileNumber - sizeX + 1].type > -1){
					map[tileNumber - sizeX + 1].type++;
					map[tileNumber - sizeX + 1].color = al_map_rgb(255, 152, 0);
				}
				if(location.y < sizeY - 1 && map[tileNumber + sizeX + 1].type > -1){
					map[tileNumber + sizeX + 1].type++;
					map[tileNumber + sizeX + 1].color = al_map_rgb(255, 152, 0);
				}
			}
			bombsCount++;
			map[tileNumber].type = -1;
			map[tileNumber].flag = 1;
			map[tileNumber].color = al_map_rgb(33, 33, 33);
		}
	} 
	return;
}

void setSpawner(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
		if(map[tileNumber].type == -1) removeBomb(location);
		setEmpty(getLocationFromTile(playerPos));
		playerPos = tileNumber;
		map[tileNumber].color = al_map_rgb(175, 180, 43);
		map[tileNumber].flag = 1;
	} 
	return;
}

void setWall(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
		if(map[tileNumber].type == -1) removeBomb(location);
		map[tileNumber].color = al_map_rgb(97, 97, 97);
		map[tileNumber].flag = -1;
		map[tileNumber].type = -2;
	} 
	return;
}

void setWater(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
		if(map[tileNumber].type == -1) removeBomb(location);
		map[tileNumber].color = al_map_rgb(0, 151, 167);
		map[tileNumber].flag = -1;
		map[tileNumber].type = -3;
	} 
	return;
}

void setFreeSpace(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
		if(map[tileNumber].type == -1) removeBomb(location);
		map[tileNumber].color = al_map_rgb(255, 224, 178);
		map[tileNumber].flag = -1;
		map[tileNumber].type = -4;
	} 
	return;
}

void setRandom(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
		if(map[tileNumber].type == -1) removeBomb(location);
		map[tileNumber].color = al_map_rgb(93, 64, 55);
		map[tileNumber].flag = -1;
		map[tileNumber].type = -5;
	} 
	return;
}