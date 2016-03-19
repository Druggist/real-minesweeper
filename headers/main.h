#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <iostream> 
#include <time.h>   
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include "global.h"

using namespace std;

extern ALLEGRO_DISPLAY *window;
extern ALLEGRO_EVENT_QUEUE *eventQueue;
extern ALLEGRO_EVENT event;
extern bool isExiting, isPlaying;
extern Coords size;
extern int pixels, gap;

void drawGame(int displayWidth);
void drawMenu(int displayHeight, int displayWidth);
void menuLogic();


#endif