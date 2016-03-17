#include "../headers/main.h"
//#include "../headers/player.h"
#include "../headers/map.h"

int main(int argc, char **argv) {
	srand(time(NULL));

    if(!al_init()){
      cout << "failed to initialize allegro!\n";
      return -1;   
    }

    if(!al_install_keyboard()){
    	cout << "failed to initialize keyboard!\n";
    	return -1;
    }
    al_init_primitives_addon();
  	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    ALLEGRO_DISPLAY *window = al_create_display(640, 480);

   	if(!window){
    	cout << "failed to create display!\n";
    	return -1;
  	}
    al_set_window_title( window,"Real minesweeper");
  	setLevel(10, 10, 5, 50, 5, al_get_display_width(window));
  	generateMap();
  	drawMap();
    sleep(1);
    Coords location;
    for(int i = 0; i<100; i++){
    location.x = i % 10;
    location.y = i /10;
    if(!openTile(location)) break;
    sleep(1);
}
sleep(2);
    al_destroy_display(window);
    return 0;
}