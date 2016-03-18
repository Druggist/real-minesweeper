#include "../headers/main.h"
#include "../headers/player.h"
#include "../headers/map.h"

void draw(Coords size, int pixels, int gap, int displayWidth){
	al_clear_to_color(al_map_rgb(255, 243, 224));
	int horizontalGap, verticalGap;
	Coords start, end;
	int centered = (displayWidth > size.x * pixels + (size.x - 1) * gap)?((displayWidth - (size.x * pixels + (size.x - 1) * gap))/2):(0);

	for(int i=0; i<size.x*size.y; i++) {
		if(map[i].location.x == 0) verticalGap = 0;
		else verticalGap = gap;
		if(map[i].location.y == 0) horizontalGap = 0;
		else horizontalGap = gap;
		start.x = map[i].location.x * pixels + verticalGap + centered;
		start.y = map[i].location.y * pixels + horizontalGap;
		end.x = map[i].location.x * pixels + pixels + centered;
		end.y = map[i].location.y * pixels + pixels;
		al_draw_filled_rectangle(start.x, start.y, end.x, end.y, map[i].color);
		if(map[i].location.x == player.location.x && map[i].location.y == player.location.y)
			al_draw_filled_rectangle(start.x + (pixels/6), start.y + (pixels/6), end.x - (pixels/6), end.y - (pixels/6), player.color);
	}
	al_flip_display();
}

int main(int argc, char **argv) {
	Coords size = {10, 10};
	int pixels = 50;
	int gap = 5;

	srand(time(NULL));

    if(!al_init()){
      cout << "failed to initialize allegro!\n";
      return -1;   
    }

    if(!al_install_keyboard()){
    	cout << "failed to initialize keyboard!\n";
    	return -1;
    }
    if(!al_init_primitives_addon()){
       	cout << "failed to initialize primitives!\n";
    	return -1;
    }

  	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    ALLEGRO_DISPLAY *window = al_create_display(640, 480);
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_set_window_title( window,"Real minesweeper");

   	if(!window){
    	cout << "failed to create display!\n";
    	return -1;
  	}
  	
  	setLevel(10, 10, 5);
  	generateMap();
  	spawnPlayer(getLocationFromTile(playerPos), bombsCount);
  	draw(size, pixels, gap, al_get_display_width(window));
  	Coords nextLocation;
  	do{
  		al_wait_for_event(eventQueue, &event);
  		if( event.type == ALLEGRO_EVENT_KEY_DOWN){
	  		if(event.keyboard.keycode  == ALLEGRO_KEY_A) {
	  			nextLocation = player.location;
	  			if(nextLocation.x > 0) nextLocation.x--;
	       		move(nextLocation);
	       		if(!openTile(player.location))  al_clear_to_color(al_map_rgb(255, 243, 224));
	       		else draw(size, pixels, gap, al_get_display_width(window));
	        	if(allTilesOpen())  al_clear_to_color(al_map_rgb(0, 0, 0));
	        } 
	        if(event.keyboard.keycode == ALLEGRO_KEY_D) {
	        	nextLocation = player.location;
	        	if(nextLocation.x < size.y -1) nextLocation.x++;
	       		move(nextLocation);
	        	if(!openTile(player.location))  al_clear_to_color(al_map_rgb(255, 243, 224));
	        	else draw(size, pixels, gap, al_get_display_width(window));
	        	if(allTilesOpen())  al_clear_to_color(al_map_rgb(0, 0, 0));
	        } 
	        if(event.keyboard.keycode == ALLEGRO_KEY_W) {
	        	nextLocation = player.location;
	        	if(nextLocation.y > 0) nextLocation.y--;
	       		move(nextLocation);
	        	if(!openTile(player.location))  al_clear_to_color(al_map_rgb(255, 243, 224));
	        	else draw(size, pixels, gap, al_get_display_width(window));
	        	if(allTilesOpen())  al_clear_to_color(al_map_rgb(0, 0, 0));
	        } 
	        if(event.keyboard.keycode == ALLEGRO_KEY_S) {
	        	nextLocation = player.location;
	        	if(nextLocation.y < size.y -1) nextLocation.y++;
	       		move(nextLocation);
	        	if(!openTile(player.location))  al_clear_to_color(al_map_rgb(255, 243, 224));
	        	else draw(size, pixels, gap, al_get_display_width(window));
	           	if(allTilesOpen())  al_clear_to_color(al_map_rgb(0, 0, 0));
	        } 
	        al_flip_display();
    	}
  	}while(event.keyboard.keycode != ALLEGRO_KEY_ESCAPE);

    al_destroy_display(window);
    al_destroy_event_queue(eventQueue);
    destroyMap();
    return 0;
}