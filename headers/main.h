#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <iostream> 
#include <time.h>   
#include <string>   
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include "global.h"

using namespace std;

extern ALLEGRO_DISPLAY *window;
extern bool isExiting, isPlaying, isEditing, inEditor;
extern Coords size;
extern int pixels, gap;

void drawGame(int displayWidth);
void drawEditor(int displayWidth);
void drawMenu(int displayHeight, int displayWidth);
void menuLogic();


#endif