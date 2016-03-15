#include "../headers/player.h"

void getInput(){
		while(!al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE)) { //koniec programu gdy wciśniemy klawisz Escape
        	al_get_keyboard_state(&keyboard);  // odczyt stanu klawiatury
   }
}