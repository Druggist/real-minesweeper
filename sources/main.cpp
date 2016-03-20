#include "../headers/main.h"
#include "../headers/player.h"
#include "../headers/map.h"
#include "../headers/gui.h"

bool isExiting = false, isPlaying = false;
Coords size;
int bombs;
int pixels = 50;
int gap = 5;
ALLEGRO_DISPLAY *window;

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
    if(!al_init_primitives_addon()){
       	cout << "failed to initialize primitives!\n";
    	return -1;
    }

  	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    window = al_create_display(640, 480);
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_set_window_title( window,"Real minesweeper");

   	if(!window){
    	cout << "failed to create display!\n";
    	return -1;
  	}
  	  	
  	templateMain();
  	drawMenu(al_get_display_height(window), al_get_display_width(window));

  	Coords nextLocation;
  	short int setFlag = 0;
  	do{
  		al_wait_for_event(eventQueue, &event);

  		if( event.type == ALLEGRO_EVENT_KEY_DOWN){

  			//flagging & questioning
  			if(event.keyboard.keycode == ALLEGRO_KEY_Q){
  				if(isPlaying){
  					setFlag = (setFlag == 0)?(3):(0);
  					player.equipmentColor = (setFlag == 3)?(al_map_rgb(93, 64, 55)):(al_map_rgb(175, 180, 43));
	       			drawGame(al_get_display_width(window));
	       		}
  			}

  			if(event.keyboard.keycode == ALLEGRO_KEY_F){ 
  				if(isPlaying){
  					setFlag = (setFlag == 0)?(2):(0);
  					player.equipmentColor = (setFlag == 2)?(al_map_rgb(183, 28, 28)):(al_map_rgb(175, 180, 43));
	       			drawGame(al_get_display_width(window));
  				}
			}

			//moving
	  		if(event.keyboard.keycode  == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
	  			if(isPlaying){
	  				nextLocation = player.location;
		  			
		  			if(nextLocation.x > 0) nextLocation.x--;
		  			if(setFlag == 0) move(nextLocation);
		  			else toggleTileFlag(nextLocation, setFlag);
	       			
	       			if(!openTile(player.location)){
	       				openAll();
	       				drawGame(al_get_display_width(window));
	       				templateStatus(false);
	       				al_rest(2.0);
	       				drawMenu(al_get_display_height(window), al_get_display_width(window));
	       				isPlaying = false;
	       			}else if(win()){
	       				drawGame(al_get_display_width(window));
	       				templateStatus(true);
	       				al_rest(2.0);
	       				drawMenu(al_get_display_height(window), al_get_display_width(window));
	       				isPlaying = false;
	       			}else{
	       				drawGame(al_get_display_width(window));
					}
	  			}else{
	  				navigateLeft();
	        		drawMenu(al_get_display_height(window), al_get_display_width(window));
	  			}
	        } 

	        if(event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode  == ALLEGRO_KEY_RIGHT) {
	  			if(isPlaying){
	  				nextLocation = player.location;
		  			
		  			if(nextLocation.x < size.x -1) nextLocation.x++;
		  			if(setFlag == 0) move(nextLocation);
		  			else toggleTileFlag(nextLocation, setFlag);
	       			
	       			if(!openTile(player.location)){
	       				openAll();
	       				drawGame(al_get_display_width(window));
	       				templateStatus(false);
	       				al_rest(2.0);
	       				drawMenu(al_get_display_height(window), al_get_display_width(window));
	       				isPlaying = false;
	       			}else if(win()){
	       				drawGame(al_get_display_width(window));
	       				templateStatus(true);
	       				al_rest(2.0);
	       				drawMenu(al_get_display_height(window), al_get_display_width(window));
	       				isPlaying = false;
	       			}else{
	       				drawGame(al_get_display_width(window));
					}
	  			}else{
	  				navigateRight();
	        		drawMenu(al_get_display_height(window), al_get_display_width(window));
	  			}
	        } 

	        if(event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode  == ALLEGRO_KEY_UP) {
	  			if(isPlaying){
	  				nextLocation = player.location;
		  			
		  			if(nextLocation.y > 0) nextLocation.y--;
		  			if(setFlag == 0) move(nextLocation);
		  			else toggleTileFlag(nextLocation, setFlag);
	       			
	       			if(!openTile(player.location)){
	       				openAll();
	       				drawGame(al_get_display_width(window));
	       				templateStatus(false);
	       				al_rest(2.0);
	       				drawMenu(al_get_display_height(window), al_get_display_width(window));
	       				isPlaying = false;
	       			}else if(win()){
	       				drawGame(al_get_display_width(window));
	       				templateStatus(true);
	       				al_rest(2.0);
	       				drawMenu(al_get_display_height(window), al_get_display_width(window));
	       				isPlaying = false;
	       			}else{
	       				drawGame(al_get_display_width(window));
					}
	  			}else{
	  				navigateUp();
	        		drawMenu(al_get_display_height(window), al_get_display_width(window));
	  			}
	        } 

	        if(event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode  == ALLEGRO_KEY_DOWN) {
	  			if(isPlaying){
	  				nextLocation = player.location;
		  			
		  			if(nextLocation.y < size.y -1) nextLocation.y++;
		  			if(setFlag == 0) move(nextLocation);
		  			else {
		  				toggleTileFlag(nextLocation, setFlag);
	       			}

	       			if(!openTile(player.location)){
	       				openAll();
	       				drawGame(al_get_display_width(window));
	       				templateStatus(false);
	       				al_rest(2.0);
	       				drawMenu(al_get_display_height(window), al_get_display_width(window));
	       				isPlaying = false;
	       			}else if(win()){
	       				drawGame(al_get_display_width(window));
	       				templateStatus(true);
	       				al_rest(2.0);
	       				drawMenu(al_get_display_height(window), al_get_display_width(window));
	       				isPlaying = false;
	       			}else{
	       				drawGame(al_get_display_width(window));
					}
	  			}else{
	  				navigateDown();
	        		drawMenu(al_get_display_height(window), al_get_display_width(window));
	  			}	        	
	        } 

	        //pausing & entering
	        if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
	        	if(isPlaying){
	        		templatePause();
	        		drawMenu(al_get_display_height(window), al_get_display_width(window));
	        		isPlaying = false;
	        	}
	        }

	        if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
	        	if(!isPlaying){
	        		menuLogic();
	        		if(!isPlaying) drawMenu(al_get_display_height(window), al_get_display_width(window));
	        		else drawGame(al_get_display_width(window));
	        	}
	        }
    	}
  	}while(!isExiting);

    al_destroy_display(window);
    al_destroy_event_queue(eventQueue);
    destroyMap();
    destroyMenu();
    return 0;
}

void drawGame(int displayWidth){
	al_clear_to_color(al_map_rgb(255, 243, 224));
	int horizontalGap, verticalGap;
	Coords start, end;
	int horizontalMargin = (displayWidth > size.x * pixels + (size.x - 1) * gap)?((displayWidth - (size.x * pixels + (size.x - 1) * gap))/2):(0);
	int verticalMargin = 100;

	for(int i=0; i<size.x*size.y; i++) {
		verticalGap = gap;
		horizontalGap = gap;
		start.x = map[i].location.x * pixels + verticalGap + horizontalMargin;
		start.y = map[i].location.y * pixels + horizontalGap + verticalMargin;
		end.x = map[i].location.x * pixels + pixels + horizontalMargin;
		end.y = map[i].location.y * pixels + pixels + verticalMargin;
		al_draw_filled_rectangle(start.x, start.y, end.x, end.y, map[i].color);
		if(map[i].location.x == player.location.x && map[i].location.y == player.location.y){
			al_draw_filled_rectangle(start.x + (pixels/6), start.y + (pixels/6), end.x - (pixels/6), end.y - (pixels/6), player.color);
			al_draw_filled_rectangle(start.x + (pixels/3), start.y + (pixels/3), end.x - (pixels/3), end.y - (pixels/3), player.equipmentColor);
		}
	}
	al_flip_display();
}

void drawMenu(int displayHeight, int displayWidth){
	al_clear_to_color(al_map_rgb(255, 243, 224));

	int horizontalMargin = displayWidth / 2;
	int verticalMargin = al_get_font_line_height(titleFont) + 50 + 100;
	int padding = 20;
	int previousWidth = 0;
	al_draw_text(titleFont, titleColor, horizontalMargin, 50, ALLEGRO_ALIGN_CENTRE, menuTitle.c_str());
	if(isHorizontal) horizontalMargin -= 50 * (rowElementsCount - 1);
	for(int i = 0; i < elementsCount; i++){
		previousWidth = 100;
		al_draw_text(menu[i].font, (menu[i].hover)?(menu[i].hoverColor):(menu[i].mainColor), menu[i].location.x * previousWidth + horizontalMargin, menu[i].location.y * (al_get_font_line_height(menu[i].font) + padding) + verticalMargin, ALLEGRO_ALIGN_CENTRE, menu[i].text.c_str());
	}
	al_flip_display();
}

void menuLogic(){
	if(menu[hoverElement].nextAction == "EXIT"){
		isExiting = true;
	}else if(menu[hoverElement].nextAction == "NEW_LOAD"){
		templateNewLoad((hoverElement == 0 || hoverElement == 2)?(true):(false));
	} else if(menu[hoverElement].nextAction == "NEW_GAME"){
		templateNewGame();
	} else if(menu[hoverElement].nextAction == "NEW_MAP"){
		//TODO EDITOR NEW MAP
	} else if(menu[hoverElement].nextAction == "LOAD_GAME"){
		if(!loadMap(window, "")){
			cout << "Coudn't load the map";
		}else{
		size.x = sizeX;
		size.y = sizeY;
		bombs = bombsCount;
		spawnPlayer(getLocationFromTile(playerPos), bombs);
		isPlaying = true;
		}
	} else if(menu[hoverElement].nextAction == "LOAD_MAP"){
		//TODO EDITOR LOAD MAP
		if(!loadMap(window, "")){
			cout << "Coudn't load the map";
		}
	} else if(menu[hoverElement].nextAction == "MAIN"){
		templateMain();
	} else if(menu[hoverElement].nextAction == "ARCADE"){
		templateArcade();
	} else if(menu[hoverElement].nextAction == "RETRO"){
		templateRetro();
	} else if(menu[hoverElement].nextAction == "1"){
		//TODO GAME LOAD ARCADE MAP
	} else if(menu[hoverElement].nextAction == "2"){
		//TODO GAME LOAD ARCADE MAP
	} else if(menu[hoverElement].nextAction == "3"){
		//TODO GAME LOAD ARCADE MAP
	} else if(menu[hoverElement].nextAction == "4"){
		//TODO GAME LOAD ARCADE MAP
	} else if(menu[hoverElement].nextAction == "5"){
		//TODO GAME LOAD ARCADE MAP
	} else if(menu[hoverElement].nextAction == "6"){
		//TODO GAME LOAD ARCADE MAP
	} else if(menu[hoverElement].nextAction == "7"){
		//TODO GAME LOAD ARCADE MAP
	} else if(menu[hoverElement].nextAction == "8"){
		//TODO GAME LOAD ARCADE MAP
	} else if(menu[hoverElement].nextAction == "9"){
		//TODO GAME LOAD ARCADE MAP
	} else if(menu[hoverElement].nextAction == "10"){
		//TODO GAME LOAD ARCADE MAP
	} else if(menu[hoverElement].nextAction == "EASY"){
		size.x = 10;
		size.y = 10;
		bombs = 20;
		setLevel(size.x, size.y, bombs);
  		generateMap();
  		spawnPlayer(getLocationFromTile(playerPos), bombs);
		isPlaying = true;
	} else if(menu[hoverElement].nextAction == "MEDIUM"){
		size.x = 15;
		size.y = 10;
		bombs = 40;
		setLevel(size.x, size.y, bombs);
  		generateMap();
  		spawnPlayer(getLocationFromTile(playerPos), bombs);
		isPlaying = true;
	} else if(menu[hoverElement].nextAction == "HARD"){
		size.x = 20;
		size.y = 20;
		bombs = 150;
		setLevel(size.x, size.y, bombs);
  		generateMap();
  		spawnPlayer(getLocationFromTile(playerPos), bombs);
		isPlaying = true;
	} else if(menu[hoverElement].nextAction == "CUSTOM"){
		size.x = 2;
		size.y = 2;
		bombs = 1;
		templateCustom();
	} else if(menu[hoverElement].nextAction == "SUBSTRACT_SIZE_X"){
		if(size.x > 2) size.x--;
		setText(4, to_string(size.x));		
	} else if(menu[hoverElement].nextAction == "SUBSTRACT_SIZE_Y"){
		if(size.y > 2) size.y--;	
		setText(10, to_string(size.y));		
	} else if(menu[hoverElement].nextAction == "SUBSTRACT_BOMBS"){
		if(bombs > 1) bombs--;	
		setText(16, to_string(bombs));		
	} else if(menu[hoverElement].nextAction == "ADD_SIZE_X"){
		if(size.x < 100) size.x++;	
		setText(4, to_string(size.x));		
	} else if(menu[hoverElement].nextAction == "ADD_SIZE_Y"){
		if(size.y < 100) size.y++;
		setText(10, to_string(size.y));		
	} else if(menu[hoverElement].nextAction == "ADD_BOMBS"){
		if(bombs < size.x * size.y - 2) bombs++;
		setText(16, to_string(bombs));		
	} else if(menu[hoverElement].nextAction == "START_CUSTOM"){
		setLevel(size.x, size.y, bombs);
  		generateMap();
  		spawnPlayer(getLocationFromTile(playerPos), bombsCount);
		isPlaying = true;						
	} else if(menu[hoverElement].nextAction == "NEW_GAME"){
		templateNewGame();
	} else if(menu[hoverElement].nextAction == "RESUME"){
		isPlaying =	true;
	} else if(menu[hoverElement].nextAction == "SAVE_MAP"){
		if(!saveMap(window)){
			cout << "Coudn't save the map";
		}
	}
}