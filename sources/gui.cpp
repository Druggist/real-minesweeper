#include "../headers/gui.h"

int elementsCount, hoverElement;
MenuElement *menu;
string menuTitle;
ALLEGRO_COLOR titleColor;
ALLEGRO_FONT *titleFont;
bool isHorizontal;

void initMenu(int e, string t){
	al_init_font_addon();
	al_init_ttf_addon();
	elementsCount = e;
	menu = new MenuElement[elementsCount];
	menuTitle = t;
	titleColor = al_map_rgb(230, 81, 0);
  	titleFont = al_load_font("../resources/fonts/bombing.ttf", 120, 0);;
	return;
}

void generateMenu(bool horizontalMenu){
	for(int i = 0; i < elementsCount; i++){
		if(horizontalMenu){
			menu[i].location.x = i % 5;
			menu[i].location.y = i / 5;
			isHorizontal = true;
		}else{
			menu[i].location.x = 0;
			menu[i].location.y = i;
			isHorizontal = false;
		}
		menu[i].mainColor = al_map_rgb(230, 81, 0);
		menu[i].hoverColor = al_map_rgb(183, 28, 28);
		menu[i].font = al_load_font("../resources/fonts/bombing.ttf", 72, 0);
		menu[i].hover = (i == 0)?(true):(false);
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

bool navigate(Coords nextLocation){
	int nextElement = elementExistsCoords(nextLocation);
	if(nextElement != -1){
		menu[hoverElement].hover = false;
		menu[nextElement].hover = true;
		hoverElement = nextElement;
		return true;
	}
	return false;
}

void navigateUp(){
	Coords nextLocation;
	nextLocation.x = menu[hoverElement].location.x;
	nextLocation.y = menu[hoverElement].location.y - 1;
	if(!navigate(nextLocation)){
		nextLocation.x = 0;
		nextLocation.y = menu[hoverElement].location.y - 1;
		navigate(nextLocation);
	}
}

void navigateDown(){
	Coords nextLocation;
	nextLocation.x = menu[hoverElement].location.x;
	nextLocation.y = menu[hoverElement].location.y + 1;
	if(!navigate(nextLocation)){
		nextLocation.x = 0;
		nextLocation.y = menu[hoverElement].location.y + 1;
		navigate(nextLocation);
	}
}

void navigateLeft(){
	Coords nextLocation;
	nextLocation.x = menu[hoverElement].location.x - 1;
	nextLocation.y = menu[hoverElement].location.y;
	if(!navigate(nextLocation)){
		nextLocation.x = 4;
		nextLocation.y = menu[hoverElement].location.y - 1;
		navigate(nextLocation);
	}
}

void navigateRight(){
	Coords nextLocation;
	nextLocation.x = menu[hoverElement].location.x + 1;
	nextLocation.y = menu[hoverElement].location.y;
	if(!navigate(nextLocation)){
		nextLocation.x = 0;
		nextLocation.y = menu[hoverElement].location.y + 1;
		navigate(nextLocation);
	}
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
	generateMenu(false);
	setText(0, "Game");
	setNextAction(0, "NEW_LOAD");
	setText(1, "Editor");
	setNextAction(1, "NEW_LOAD");
	setText(2, "Exit");
	setNextAction(2, "EXIT");
}

void templateNewLoad(bool game){
	destroyMenu();
	initMenu(3, (game)?("Start game"):("Create map"));
	generateMenu(false);
	setText(0, "New");
	setNextAction(0, (game)?("NEW_GAME"):("NEW_MAP"));
	setText(1, "Load");
	setNextAction(1, (game)?("LOAD_GAME"):("LOAD_MAP"));
	setText(2, "Back");
	setNextAction(2, "MAIN");
}

void templateNewGame(){
	destroyMenu();
	initMenu(3, "New game");
	generateMenu(false);
	setText(0, "Arcade");
	setNextAction(0, "ARCADE");
	setText(1, "Retro");
	setNextAction(1, "RETRO");
	setText(2, "Back");
	setNextAction(2, "NEW_LOAD");
}

void templateArcade(){
	destroyMenu();
	initMenu(11, "Levels");
	generateMenu(true);
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
	generateMenu(false);
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
	generateMenu(false);
	setText(0, "Resume");
	setNextAction(0, "RESUME");
	setText(1, "Save map");
	setNextAction(1, "SAVE_MAP");
	setText(2, "Main menu");
	setNextAction(2, "MAIN");
}

void templateStatus(bool win){
	destroyMenu();
	initMenu(2, (win)?("You win"):("You lost"));
	generateMenu(false);
	setText(0, "Save map");
	setNextAction(0, "SAVE_MAP");
	setText(1, "Main menu");
	setNextAction(1, "NEW_GAME");
}
