#include "../headers/main.h"
#include "../headers/player.h"
#include "../headers/map.h"

int main(int argc, char **argv) {
    al_init();
    al_install_keyboard();
    *window = al_create_display( 640, 480);
    al_set_window_title( window,"Real minesweeper");
    al_destroy_display(window);
    return 0;
}