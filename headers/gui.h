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
	string text;
	string nextAction;
	bool hover;
};

extern MenuElement *menu;
extern int elementsCount, hoverElement;
extern string menuTitle;
extern ALLEGRO_COLOR titleColor;
extern ALLEGRO_FONT *titleFont;
extern bool isHorizontal;

void initMenu(int e, string t);
void generateMenu(bool horizontalMenu);  //type true horizontal false vertical
void setText(int element, string text);
void setNextAction(int element, string nextAction);
void navigateUp();
void navigateDown();
void navigateLeft();
void navigateRight();
bool navigate(Coords nextLocation);
int elementExistsCoords(Coords location);
void destroyMenu();
//tempalates
void templateMain();
void templateNewLoad(bool game);
void templateNewGame();
void templateArcade();
void templateRetro();
void templatePause();
void templateStatus(bool win);

#endif