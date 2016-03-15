#include "../headers/main.h"
#include "../headers/player.h"
#include "../headers/map.h"

int main(int argc, char **argv) {
    al_init(); // inicjowanie biblioteki allegro
    al_install_keyboard(); // instalowanie sterownika klawiatury
    // utworzenie struktury do odczytu stanu klawiatury
    *window = al_create_display( 640, 480);// tworzymy wskaźnik okna, i podajemy jego szer. i wys
    al_set_window_title( window,"Real minesweeper");// podajemy tytuł okna
     
    // usuwanie z pamięci okna, bitmap, audio, fontów ...itd.
    al_destroy_display(window);
    return 0;
}