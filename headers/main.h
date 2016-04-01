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

void drawMenu();
void drawGame();
void drawGameGui();
void drawEditor();
void drawEditorGui();
void menuLogic();
void playTileSound();
void stopTileSound();
void playerMovement(Coords nextLocation);
void startPlaying();
void loadGame(string map);
void newGame();

#endif