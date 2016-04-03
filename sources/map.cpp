#include "../headers/map.h"

short int sizeX, sizeY, bombsCount, bombsCountEditor, flaggedBombs, openTiles;
int displayWidth;
Tile *map;
Colors mapColors;
short int *bombsPos, playerPos;  

void setLevel(int x, int y, int b){
	sizeX = x;
	sizeY = y;
	bombsCount = b;
	bombsCountEditor = b;
	flaggedBombs = 0;
	openTiles = 1;

	mapColors.close = al_map_rgb(230, 81, 0);
	mapColors.flag = al_map_rgb(183, 28, 28);
	mapColors.questionMark = al_map_rgb(93, 64, 55);
	mapColors.questionMarkFont = al_load_font("../resources/fonts/bombing.ttf", 32, 0);
	mapColors.questionMarkFontColor = al_map_rgb(188, 170, 164);
	mapColors.bomb = al_map_rgb(33, 33, 33);
	mapColors.spawner = al_map_rgb(175, 180, 43);
	mapColors.freeSpace = al_map_rgb(255, 243, 224);
	mapColors.freeSpaceEditor = al_map_rgb(255, 224, 178);
	mapColors.wall = al_map_rgb(97, 97, 97);
	mapColors.water = al_map_rgb(0, 151, 167);
	mapColors.empty = al_map_rgb(255, 204, 128);
	mapColors.nearBomb = al_map_rgb(255, 152, 0);
	return;
}

bool createMap(){
	destroyMap();
	int size = sizeX * sizeY;
	if(size > 0){
		map = new Tile[size];
		for(int i = 0; i < size; i++){
			map[i].location.x = i % sizeX;
			map[i].location.y = i / sizeX;
			map[i].type = -5;
			map[i].flag = 1;
			map[i].color = mapColors.questionMark;
		}
		if(!setSpawner(generateSpawnerLocation())) return false;
	} else return false;
	return true;
}

void setUntriggerableColors(){
	for(int i = 0; i < sizeX * sizeY; i++){
		if(map[i].flag == -1){
			switch(map[i].type){
				case -2:
					map[i].color = mapColors.wall;
					break;
				case -3:
					map[i].color = mapColors.water;
					break;	
				case -4:
					map[i].color = mapColors.freeSpace;
					break;
				default:
					break;
			}
		}
	}
}

bool fillRandomTiles(){
	int randomTiles = getRandomTilesCount();
	int bombsToPlace = bombsCount - getPlacedBombsCount();
	int countBombs = 0;
	if(randomTiles >= bombsToPlace && bombsToPlace >= 0){
		if(!generateBombs(randomTiles, bombsToPlace)) return false;
		for(int i = 0; i < randomTiles; i++){
			int randomTile = getFirstRandomTile();
			if(i == bombsPos[countBombs]){
				setBomb(getLocationFromTile(randomTile));
				countBombs++;
			} else {
				setEmpty(getLocationFromTile(randomTile));
			}
		}
	} else return false;
	return true;
}

void closeAll(){
	for(int i = 0; i < sizeX * sizeY; i++){
		if(i != playerPos && map[i].flag != -1){
			map[i].flag = 0;
			map[i].color = mapColors.close;
		}
	}
}

int getFirstRandomTile(){
	for(int i = 0; i < sizeX * sizeY; i++){
		if(map[i].type == -5) return i;
	}
	return -1;
}

bool generateMap(){
	if(!createMap()) return false;
	if(!fillRandomTiles()) return false;
	closeAll();
	return true;
}

bool generateBombs(int randomElements, int bombsQuantity){
	if(bombsQuantity <= randomElements && bombsQuantity >= 0 && randomElements > 0){
		if(bombsQuantity > 0){
			bombsPos = new short int[bombsQuantity];
			for(int i = 0; i < bombsQuantity; i++){
				bombsPos[i] = rand() % (randomElements);
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

Coords generateSpawnerLocation(){
	playerPos = rand() % (sizeX * sizeY);
	while(map[playerPos].type < 0 && map[playerPos].type != -5) playerPos = rand() % (sizeX * sizeY);
	return getLocationFromTile(playerPos);
}

bool openTile(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(map[tileNumber].flag == 0){
		map[tileNumber].flag = 1;
		switch(map[tileNumber].type){
			case -1:
			map[tileNumber].color = mapColors.bomb;
			return false;
			break;		
	
			case 0:
			map[tileNumber].color = mapColors.empty;
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
			map[tileNumber].color = mapColors.nearBomb;
			break;
		}
		openTiles++;
	}
	return true;
}

int getTileFromLocation(Coords location){
	int tile = location.x + sizeX * location.y;
	return (tile < sizeX * sizeY)?(tile):(-1);
}

Coords getLocationFromTile(int tileNumber){
	if(tileNumber < sizeX * sizeY && tileNumber >= 0) return map[tileNumber].location;
	Coords location= {0, -1};
	return location;
}

int getNormalTilesCount(){
	int count = 0;
	for(int i = 0; i < sizeX * sizeY; i++){
		if(map[i].type >= 0) count++;
	}
	return count;
}

int getPlacedBombsCount(){
	int count = 0;
	for(int i = 0; i < sizeX * sizeY; i++){
		if(map[i].type == -1) count++;
	}
	return count;
}

int getRandomTilesCount(){
	int count = 0;
	for(int i = 0; i < sizeX * sizeY; i++){
		if(map[i].type == -5) count++;
	}
	return count;
}

bool allTilesOpen(){
	if(openTiles == getNormalTilesCount()) return true;
	return false;
}

bool allBombsFlagged(){
	if(flaggedBombs == bombsCount) return true;
	return false;
}

bool canMove(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber >= 0 && tileNumber < sizeX * sizeY){
		if(map[tileNumber].flag == -1) return false;
	} else return false;
	return true;
}

bool win(){
	if(allBombsFlagged() || allTilesOpen()) return true;
	return false;
}

int toggleTileFlag(Coords location, short int flag){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber > -1 && tileNumber < sizeX * sizeY){
		if(map[tileNumber].flag != -1 && map[tileNumber].flag != 1){
			map[tileNumber].flag = (map[tileNumber].flag != flag)?(flag):(0);
			switch(map[tileNumber].flag){
				case 2: 
					map[tileNumber].color = mapColors.flag;
					if(map[tileNumber].type == -1) flaggedBombs++;
					return 1;
					break;

				case 3:
					map[tileNumber].color = mapColors.questionMark;
					return 3;
					break;

				default:
					if(map[tileNumber].type == -1 && flag == 2) flaggedBombs--;
					map[tileNumber].color = mapColors.close;
					return 2;
					break;
			}
		}
	}
	return 0;
}

void openAll(){
	for(int i = 0; i < sizeX * sizeY; i++){
		if(map[i].flag > 0 && i != playerPos) map[i].flag = 0;
		openTile(getLocationFromTile(i));
	}
	return;
}

bool saveMap(ALLEGRO_DISPLAY *display){
	if(!al_init_native_dialog_addon()) return false;
	ALLEGRO_FILECHOOSER *filechooser;
	filechooser = al_create_native_file_dialog("../resources/maps/new.map", "Save map.", "*.map;", ALLEGRO_FILECHOOSER_SAVE);
	al_show_native_file_dialog(display, filechooser);
	if(al_get_native_file_dialog_count(filechooser) > 0){
		fstream file;
		file.open(al_get_native_file_dialog_path(filechooser, 0), ios::out);
		if(!file.good()) return false;
		file <<sizeX <<" " <<sizeY <<" " <<bombsCount + bombsCountEditor <<endl;
		file <<playerPos <<endl;
		for(int i = 0; i < sizeX * sizeY; i++){
			short int flag = (i == playerPos)?(map[i].flag):((map[i].flag == -1)?(-1):(0));
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
		filechooser = al_create_native_file_dialog("../resources/maps/", "Load map.", "*.map;", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
		if(!al_show_native_file_dialog(display, filechooser)) return false;
		if(al_get_native_file_dialog_count(filechooser) > 0) path = al_get_native_file_dialog_path(filechooser, 0);
		else return false;
	}
	fstream file;
	file.open(path, ios::in);
	if(!file.good()) return false;
	destroyMap();
	string data;
	setLevel(0, 0, 0);
	getline(file, data, ' ');
	sizeX = atoi(data.c_str());
	if(sizeX > 100 || sizeX < 2) return false;
	getline(file, data, ' ');
	sizeY = atoi(data.c_str());
	if(sizeY > 100 || sizeY < 2) return false;
	getline(file, data);
	bombsCount = atoi(data.c_str());
	bombsCountEditor = atoi(data.c_str());
	if(bombsCount > sizeX * sizeY - 2 || bombsCount < 0) return false;
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
	}
	setUntriggerableColors();
	setSpawner(getLocationFromTile(playerPos));
	file.close();
	al_shutdown_native_dialog_addon();
	return true;
}

void setEditorColors(){
	for(int i = 0; i < sizeX * sizeY; i++){
		if(playerPos != i){
			if(map[i].type == -5) map[i].color = mapColors.questionMark;
			if(map[i].type == -4) map[i].color = mapColors.freeSpaceEditor;
		}
	}
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
		map[tileNumber].color = (map[tileNumber].type == 0)?(mapColors.empty):(mapColors.nearBomb);
		setSpawner(getLocationFromTile(playerPos));
	} 
	return;
}

void removeBomb(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
		if(map[tileNumber].type == -1){ 
			//left side 
			if(location.x > 0){
				if(map[tileNumber - 1].type > 0){
					map[tileNumber - 1].type--;
					map[tileNumber - 1].color = (map[tileNumber - 1].type == 0)?(mapColors.empty):(mapColors.nearBomb);
				}
				if(location.y > 0 && map[tileNumber - sizeX - 1].type > 0){
					map[tileNumber - sizeX - 1].type--;
					map[tileNumber - sizeX - 1].color = (map[tileNumber - sizeX - 1].type == 0)?(mapColors.empty):(mapColors.nearBomb);
				}
				if(location.y < sizeY - 1 && map[tileNumber + sizeX - 1].type > 0) {
					map[tileNumber + sizeX - 1].type--;
					map[tileNumber + sizeX - 1].color = (map[tileNumber + sizeX - 1].type == 0)?(mapColors.empty):(mapColors.nearBomb);
				}
			}

			//center
			if(location.y > 0 && map[tileNumber - sizeX].type > 0) {
				map[tileNumber - sizeX].type--;
				map[tileNumber - sizeX].color = (map[tileNumber - sizeX].type == 0)?(mapColors.empty):(mapColors.nearBomb);
			}
			if(location.y < sizeY - 1 && map[tileNumber + sizeX].type > 0){
				map[tileNumber + sizeX].type--;
				map[tileNumber + sizeX].color = (map[tileNumber + sizeX].type == 0)?(mapColors.empty):(mapColors.nearBomb);
			}

			//right side
			if(location.x < sizeX - 1){
				if(map[tileNumber + 1].type > 0){
					map[tileNumber + 1].type--;
					map[tileNumber + 1].color = (map[tileNumber + 1].type == 0)?(mapColors.empty):(mapColors.nearBomb);
				}
				if(location.y > 0 && map[tileNumber - sizeX + 1].type > 0){
					map[tileNumber - sizeX + 1].type--;
					map[tileNumber - sizeX + 1].color = (map[tileNumber - sizeX + 1].type == 0)?(mapColors.empty):(mapColors.nearBomb);
				}
				if(location.y < sizeY - 1 && map[tileNumber + sizeX + 1].type > 0){
					map[tileNumber + sizeX + 1].type--;
					map[tileNumber + sizeX + 1].color = (map[tileNumber + sizeX + 1].type == 0)?(mapColors.empty):(mapColors.nearBomb);
				}
			}
			bombsCountEditor--;
			map[tileNumber].type = checkNeighbours(location);
			map[tileNumber].flag = 1;
			map[tileNumber].color = (map[tileNumber].type == 0)?(mapColors.empty):(mapColors.nearBomb); 
			setSpawner(getLocationFromTile(playerPos));
		}
	} 
	return;
}

void setBomb(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
		if(map[tileNumber].type != -1 && tileNumber != playerPos){ 
			//left side 
			if(location.x > 0){
				if(map[tileNumber - 1].type > -1){
					map[tileNumber - 1].type++;
					map[tileNumber - 1].color = mapColors.nearBomb;
				}
				if(location.y > 0 && map[tileNumber - sizeX - 1].type > -1){
					map[tileNumber - sizeX - 1].type++;
					map[tileNumber - sizeX - 1].color = mapColors.nearBomb;
				}
				if(location.y < sizeY - 1 && map[tileNumber + sizeX - 1].type > -1) {
					map[tileNumber + sizeX - 1].type++;
					map[tileNumber + sizeX - 1].color = mapColors.nearBomb;
				}
			}

			//center
			if(location.y > 0 && map[tileNumber - sizeX].type > -1) {
				map[tileNumber - sizeX].type++;
				map[tileNumber - sizeX].color = mapColors.nearBomb;
			}
			if(location.y < sizeY - 1 && map[tileNumber + sizeX].type > -1){
				map[tileNumber + sizeX].type++;
				map[tileNumber + sizeX].color = mapColors.nearBomb;
			}

			//right side
			if(location.x < sizeX - 1){
				if(map[tileNumber + 1].type > -1){
					map[tileNumber + 1].type++;
					map[tileNumber + 1].color = mapColors.nearBomb;
				}
				if(location.y > 0 && map[tileNumber - sizeX + 1].type > -1){
					map[tileNumber - sizeX + 1].type++;
					map[tileNumber - sizeX + 1].color = mapColors.nearBomb;
				}
				if(location.y < sizeY - 1 && map[tileNumber + sizeX + 1].type > -1){
					map[tileNumber + sizeX + 1].type++;
					map[tileNumber + sizeX + 1].color = mapColors.nearBomb;
				}
			}
			bombsCountEditor++;
			map[tileNumber].type = -1;
			map[tileNumber].flag = 1;
			map[tileNumber].color = mapColors.bomb;
			setSpawner(getLocationFromTile(playerPos));
		}
	} 
	return;
}

bool setSpawner(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1){
		if(tileNumber != playerPos){
			if(map[tileNumber].type == -1) removeBomb(location);
			int oldPlayerPos = playerPos;
			playerPos = tileNumber;
			setEmpty(getLocationFromTile(oldPlayerPos));
		}
			map[playerPos].color = mapColors.spawner;
			map[playerPos].flag = 1;
	} else return false;
	return true;
}

void setWall(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1 && tileNumber != playerPos){
		if(map[tileNumber].type == -1) removeBomb(location);
		map[tileNumber].color = mapColors.wall;
		map[tileNumber].flag = -1;
		map[tileNumber].type = -2;
	} 
	return;
}

void setWater(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1 && tileNumber != playerPos){
		if(map[tileNumber].type == -1) removeBomb(location);
		map[tileNumber].color = mapColors.water;
		map[tileNumber].flag = -1;
		map[tileNumber].type = -3;
	} 
	return;
}

void setFreeSpace(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1 && tileNumber != playerPos){
		if(map[tileNumber].type == -1) removeBomb(location);
		map[tileNumber].color = mapColors.freeSpaceEditor;
		map[tileNumber].flag = -1;
		map[tileNumber].type = -4;
	} 
	return;
}

void setRandom(Coords location){
	int tileNumber = getTileFromLocation(location);
	if(tileNumber != -1 && tileNumber != playerPos){
		if(map[tileNumber].type == -1) removeBomb(location);
		map[tileNumber].color = mapColors.questionMark;
		map[tileNumber].flag = 1;
		map[tileNumber].type = -5;
	} 
	return;
}