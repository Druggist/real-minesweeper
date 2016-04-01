#ifndef MAIN_H
#define MAIN_H

#include "global.h"
#include <cstdlib>
#include <ctime>   
#include <string>   
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;

extern bool isExiting, isPlaying, isEditing, inEditor;
extern ALLEGRO_DISPLAY *window;
extern Coords size;
extern int pixels, gap;

void drawGame(int displayWidth);
void drawEditor(int displayWidth);
void drawMenu(int displayHeight, int displayWidth);
void menuLogic();
void playTileSound();
void stopTileSound();

#endif