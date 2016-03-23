#include "../headers/gui.h"

int elementsCount, hoverElement;
MenuElement *menu;
string menuTitle;
ALLEGRO_COLOR titleColor;
ALLEGRO_FONT *titleFont;
bool isHorizontal;
short int rowElementsCount;

void initMenu(int e, string t){
	al_init_font_addon();
	al_init_ttf_addon();
	elementsCount = e;
	menu = new MenuElement[elementsCount];
	menuTitle = t;
	titleColor = al_map_rgb(255, 204, 128);
  	titleFont = al_load_font("../resources/fonts/bombing.ttf", 120, 0);;
	return;
}

void generateMenu(bool horizontalMenu, short int elementsInRow){
	rowElementsCount = elementsInRow;
	for(int i = 0; i < elementsCount; i++){
		if(horizontalMenu){
			menu[i].location.x = i % elementsInRow;
			menu[i].location.y = i / elementsInRow;
			isHorizontal = true;
		}else{
			menu[i].location.x = 0;
			menu[i].location.y = i;
			isHorizontal = false;
		}
		menu[i].mainColor = al_map_rgb(230, 81, 0);
		menu[i].hoverColor = al_map_rgb(183, 28, 28);
		menu[i].font = al_load_font("../resources/fonts/bombing.ttf", 64, 0);
		menu[i].smallerFont = al_load_font("../resources/fonts/bombing.ttf", 48, 0);
		menu[i].hover = (i == 0)?(true):(false);
		menu[i].enabled = true;
		hoverElement = 0;
	}
}

void setText(int element, string text){
	if(element < elementsCount) menu[element].text = text;
	return;
}

void setNextAction(int element, string nextAction){
	if(element < elementsCount) menu[element].nextAction = nextAction;
	return;
}

void setEnabled(int element, bool enable){
	if(element < elementsCount){
		menu[element].enabled = enable;
		menu[element].mainColor = al_map_rgb(255, 204, 128);
	}
	return;
}

int navigate(Coords nextLocation){
	int nextElement = elementExistsCoords(nextLocation);
	if(nextElement != -1){
		if(menu[nextElement].enabled == true){
		menu[hoverElement].hover = false;
		menu[nextElement].hover = true;
		hoverElement = nextElement;
		return 1;
	}else return 0;
	}
	return -1;
}

void navigateUp(){
	Coords nextLocation;
	bool isEnabled = false, checkedForFirst = false;
		nextLocation.x = menu[hoverElement].location.x;
		nextLocation.y = menu[hoverElement].location.y - 1;
	do{
		int navigation = navigate(nextLocation);
		if(navigation == -1){
			nextLocation.x = 0;
			if(checkedForFirst == true) isEnabled = true;
			if( elementExistsCoords(nextLocation) == -1 ) checkedForFirst = true;
		} else if(navigation == 0){
			nextLocation.y--;
		} else isEnabled = true;
	}while(!isEnabled);
}

void navigateDown(){
	Coords nextLocation;
	bool isEnabled = false, checkedForFirst = false;
		nextLocation.x = menu[hoverElement].location.x;
		nextLocation.y = menu[hoverElement].location.y + 1;
	do{
		int navigation = navigate(nextLocation);
		if(navigation == -1){
			nextLocation.x = 0;
			if(checkedForFirst == true) isEnabled = true;
			if( elementExistsCoords(nextLocation) == -1 ) checkedForFirst = true;
		} else if(navigation == 0){
			nextLocation.y++;
		} else isEnabled = true;
	}while(!isEnabled);
}

void navigateLeft(){
	Coords nextLocation;
	bool isEnabled = false, checkedForFirst = false;
		nextLocation.x = menu[hoverElement].location.x - 1;
		nextLocation.y = menu[hoverElement].location.y;
	do{
		int navigation = navigate(nextLocation);
		if(navigation == -1){
			nextLocation.x = rowElementsCount - 1;
			nextLocation.y--;
			if(checkedForFirst == true) isEnabled = true;
			if( elementExistsCoords(nextLocation) == -1 ) checkedForFirst = true;
		} else if(navigation == 0){
			nextLocation.x--;
		} else isEnabled = true;
	}while(!isEnabled);
}

void navigateRight(){
	Coords nextLocation;
	bool isEnabled = false, checkedForFirst = false;
		nextLocation.x = menu[hoverElement].location.x + 1;
		nextLocation.y = menu[hoverElement].location.y;
	do{
		int navigation = navigate(nextLocation);
		if(navigation == -1){
			nextLocation.x = 0;
			nextLocation.y++;
			if(checkedForFirst == true) isEnabled = true;
			if( elementExistsCoords(nextLocation) == -1 ) checkedForFirst = true;
		} else if(navigation == 0){
			nextLocation.x++;
		} else isEnabled = true;
	}while(!isEnabled);
}

int elementExistsCoords(Coords location){
	for(int i = 0; i < elementsCount; i++){
		if(location.x == menu[i].location.x && location.y == menu[i].location.y) return i;
	}
	return -1;
}

void destroyMenu(){
	if(menu != NULL) delete[] menu;
	return;
}

void templateMain(){
	destroyMenu();
	initMenu(3, "Main menu");
	generateMenu(false, 0);
	setText(0, "Game");
	setNextAction(0, "NEW_LOAD_GAME");
	setText(1, "Editor");
	setNextAction(1, "NEW_LOAD_MAP");
	setText(2, "Exit");
	setNextAction(2, "EXIT");
}

void templateNewLoadGame(){
	destroyMenu();
	initMenu(3, "Start game");
	generateMenu(false, 0);
	setText(0, "New");
	setNextAction(0, "NEW_GAME");
	setText(1, "Load");
	setNextAction(1, "LOAD_GAME");
	setText(2, "Back");
	setNextAction(2, "MAIN");
}

void templateNewLoadMap(){
	destroyMenu();
	initMenu(3, "Create map");
	generateMenu(false, 0);
	setText(0, "New");
	setNextAction(0, "NEW_MAP");
	setText(1, "Load");
	setNextAction(1, "LOAD_MAP");
	setText(2, "Back");
	setNextAction(2, "MAIN");
}

void templateNewGame(){
	destroyMenu();
	initMenu(3, "New game");
	generateMenu(false, 0);
	setText(0, "Arcade");
	setNextAction(0, "ARCADE");
	setText(1, "Retro");
	setNextAction(1, "RETRO");
	setText(2, "Back");
	setNextAction(2, "NEW_LOAD_GAME");
}

void templateArcade(){
	destroyMenu();
	initMenu(11, "Levels");
	generateMenu(true, 5);
	setText(0, "1");
	setNextAction(0, "1");
	setText(1, "2");
	setNextAction(1, "2");
	setText(2, "3");
	setNextAction(2, "3");
	setText(3, "4");
	setNextAction(3, "4");
	setText(4, "5");
	setNextAction(4, "5");
	setText(5, "6");
	setNextAction(5, "6");
	setText(6, "7");
	setNextAction(6, "7");
	setText(7, "8");
	setNextAction(7, "8");
	setText(8, "9");
	setNextAction(8, "9");
	setText(9, "10");
	setNextAction(9, "10");							
	setText(10, "Back");
	setNextAction(10, "NEW_GAME");
}

void templateRetro(){
	destroyMenu();
	initMenu(5, "Difficulty");
	generateMenu(false, 0);
	setText(0, "Easy");
	setNextAction(0, "EASY");
	setText(1, "Medium");
	setNextAction(1, "MEDIUM");
	setText(2, "Hard");
	setNextAction(2, "HARD");
	setText(3, "Custom");
	setNextAction(3, "CUSTOM");						
	setText(4, "Back");
	setNextAction(4, "NEW_GAME");
}

void templatePause(){
	destroyMenu();
	initMenu(3, "Pause");
	generateMenu(false, 0);
	setText(0, "Resume");
	setNextAction(0, "RESUME");
	setText(1, "Save map");
	setNextAction(1, "SAVE_MAP");
	setText(2, "Main menu");
	setNextAction(2, "MAIN");
}

void templatePauseEditor(){
	destroyMenu();
	initMenu(3, "Pause");
	generateMenu(false, 0);
	setText(0, "Back to editing");
	setNextAction(0, "RESUME");
	setText(1, "Save map");
	setNextAction(1, "SAVE_MAP_EDITOR");
	setText(2, "Main menu");
	setNextAction(2, "MAIN");
}

void templateBombsQuantity(){
	destroyMenu();
	initMenu(9, "Save map");
	generateMenu(true, 3);
	setText(0, "Bombs:");
	setEnabled(0, false);
	menu[0].hover = false;
	setEnabled(1, false);
	setEnabled(2, false);
	setText(3, "<<");
	setNextAction(3, "SUBSTRACT_BOMBS");
	menu[3].hover = true;
	hoverElement = 3;
	setText(4, "");
	setEnabled(4, false);
	setText(5, ">>");
	setNextAction(5, "ADD_BOMBS");
	setText(6, "Back");
	setNextAction(6, "PAUSE_EDITOR");
	setEnabled(7, false);
	setText(8, "Save");
	setNextAction(8, "SAVE_MAP");
}

void templateStatus(bool win){
	destroyMenu();
	initMenu(2, (win)?("You win"):("You lost"));
	generateMenu(false, 0);
	setText(0, "Save map");
	setNextAction(0, "SAVE_MAP");
	setText(1, "Main menu");
	setNextAction(1, "NEW_LOAD_GAME"); 
}

void templateCustom(){
	destroyMenu();
	initMenu(21, "Custom map");
	generateMenu(true, 3);
	setText(0, "Size X:");
	setEnabled(0, false);
	menu[0].hover = false;
	setEnabled(1, false);
	setEnabled(2, false);
	setText(3, "<<");
	setNextAction(3, "SUBSTRACT_SIZE_X");
	menu[3].hover = true;
	hoverElement = 3;
	setText(4, "2");
	setEnabled(4, false);
	setText(5, ">>");
	setNextAction(5, "ADD_SIZE_X");
	setText(6, "Size Y:");
	setEnabled(6, false);
	setEnabled(7, false);
	setEnabled(8, false);
	setText(9, "<<");
	setNextAction(9, "SUBSTRACT_SIZE_Y");
	setText(10, "2");
	setEnabled(10, false);
	setText(11, ">>");
	setNextAction(11, "ADD_SIZE_Y");
	setText(12, "Bombs:");
	setEnabled(12, false);
	setEnabled(13, false);
	setEnabled(14, false);
	setText(15, "<<");
	setNextAction(15, "SUBSTRACT_BOMBS");
	setText(16, "1");
	setEnabled(16, false);
	setText(17, ">>");
	setNextAction(17, "ADD_BOMBS");
	setText(18, "Back");
	setNextAction(18, "RETRO");
	setEnabled(19, false);
	setText(20, "Start");
	setNextAction(20, "START_CUSTOM");
}

void templateNewMap(){
	destroyMenu();
	initMenu(15, "Custom map");
	generateMenu(true, 3);
	setText(0, "Size X:");
	setEnabled(0, false);
	menu[0].hover = false;
	setEnabled(1, false);
	setEnabled(2, false);
	setText(3, "<<");
	setNextAction(3, "SUBSTRACT_SIZE_X");
	menu[3].hover = true;
	hoverElement = 3;
	setText(4, "2");
	setEnabled(4, false);
	setText(5, ">>");
	setNextAction(5, "ADD_SIZE_X");
	setText(6, "Size Y:");
	setEnabled(6, false);
	setEnabled(7, false);
	setEnabled(8, false);
	setText(9, "<<");
	setNextAction(9, "SUBSTRACT_SIZE_Y");
	setText(10, "2");
	setEnabled(10, false);
	setText(11, ">>");
	setNextAction(11, "ADD_SIZE_Y");
	setText(12, "Back");
	setNextAction(12, "NEW_LOAD_MAP");
	setEnabled(13, false);
	setText(14, "Start");
	setNextAction(14, "NEW_MAP_CREATE");
}
