#ifndef GUI_H
#define GUI_H 

#include "global.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>

using namespace std;

struct MenuElement{
	Coords location;
	ALLEGRO_COLOR mainColor;
	ALLEGRO_COLOR hoverColor;
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *smallerFont;
	string text;
	string nextAction;
	bool hover;
	bool enabled;
};

extern MenuElement *menu;
extern int elementsCount, hoverElement;
extern string menuTitle;
extern ALLEGRO_COLOR titleColor;
extern ALLEGRO_FONT *titleFont;
extern bool isHorizontal;
extern short int rowElementsCount;

void initMenu(int e, string t);
void generateMenu(bool horizontalMenu, short int elementsInRow);  //type true horizontal false vertical
void setText(int element, string text);
void setEnabled(int element, bool enable);
void setNextAction(int element, string nextAction);
void navigateUp();
void navigateDown();
void navigateLeft();
void navigateRight();
int navigate(Coords nextLocation);
int elementExistsCoords(Coords location);
void destroyMenu();
//tempalates
void templateMain();
void templateNewLoadGame();
void templateNewLoadMap();
void templateNewGame();
void templateNewMap();
void templateArcade();
void templateRetro();
void templatePause();
void templatePauseEditor();
void templateBombsQuantity();
void templateStatus(bool win);
void templateCustom();
void templateGameGui();

#endif