#include "../headers/main.h"
#include "../headers/player.h"
#include "../headers/map.h"
#include "../headers/gui.h"

bool isExiting = false, isPlaying = false, isEditing = false, inEditorMenu = false;
Coords size = {2, 2};
int bombs = 1, previousType = -1; 
int pixels = 50, gap = 5;
double gameTime = 0.0;
short int currentMode = 1, setFlag = 0;
ALLEGRO_DISPLAY *window = NULL;
ALLEGRO_SAMPLE *soundtrack = NULL, *setObject = NULL, *sound = NULL, *tileSound = NULL;
ALLEGRO_SAMPLE_ID soundtrackId, tileSoundId;

int main(int argc, char **argv) {
	srand(time(NULL));

	if(!al_init()){
		al_show_native_message_box(NULL, "Error", "Error #1", "Failed to initialize allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;   
	}

	if(!al_install_keyboard()){
		al_show_native_message_box(NULL, "Error", "Error #2", "Failed to initialize keyboard!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	if(!al_install_mouse()){
		al_show_native_message_box(NULL, "Error", "Error #3", "Failed to initialize mouse!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	if(!al_init_primitives_addon()){
		al_show_native_message_box(NULL, "Error", "Error #4", "Failed to initialize primitives!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	if(!al_install_audio()){
	  	al_show_native_message_box(NULL, "Error", "Error #5", "Failed to initialize audio!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	  	return -1;
	}

	if(!al_init_acodec_addon()){
	  	al_show_native_message_box(NULL, "Error", "Error #6", "Failed to initialize codecs!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	  	return -1;
   	}
 
	if (!al_reserve_samples(4)){
		al_show_native_message_box(NULL, "Error", "Error #6", "Failed to reserve samples!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	  	return -1;
	}
 
   	soundtrack = al_load_sample( "../resources/sounds/background.wav" );
   	setObject = al_load_sample( "../resources/sounds/set_item.wav" );
 
   	if (!soundtrack || !setObject){
		al_show_native_message_box(NULL, "Error", "Error #7", "Failed to load sample!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
   	}

	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	window = al_create_display(640, 480);
	ALLEGRO_EVENT event;
	ALLEGRO_TIMER *timer = al_create_timer(1.0);
	ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_mouse_event_source());
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_set_window_title( window,"Real minesweeper");

	if(!window){
		al_show_native_message_box(NULL, "Error", "Error #8", "Failed to initialize display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
		
	templateMain();
	al_play_sample(soundtrack, 0.75, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &soundtrackId);
	al_hide_mouse_cursor(window);
	drawMenu();
	
	Coords nextLocation;
	bool draw = false;
	
	al_start_timer(timer);
	while(!isExiting){
		al_wait_for_event(eventQueue, &event);
		
		if( event.type == ALLEGRO_EVENT_KEY_DOWN){
			if(!isEditing){
				al_hide_mouse_cursor(window);
				//flagging & questioning
				if(event.keyboard.keycode == ALLEGRO_KEY_Q){
					if(isPlaying){
						setFlag = (setFlag == 0)?(3):(0);
						player.equipmentColor = (setFlag == 3)?(al_map_rgb(93, 64, 55)):(al_map_rgb(175, 180, 43));
						drawGame();
						al_play_sample(setObject, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
					}
				}

				if(event.keyboard.keycode == ALLEGRO_KEY_F){ 
					if(isPlaying){
						setFlag = (setFlag == 0)?(2):(0);
						player.equipmentColor = (setFlag == 2)?(al_map_rgb(183, 28, 28)):(al_map_rgb(175, 180, 43));
						drawGame();
						al_play_sample(setObject, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
					}
				}

				//moving
				if(event.keyboard.keycode  == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
					if(isPlaying){
						nextLocation = player.location;
						if(nextLocation.x > 0) nextLocation.x--;
						playerMovement(nextLocation);	
					}else{
						navigateLeft();
						drawMenu();
					}
					al_play_sample(setObject, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
				} 

				if(event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode  == ALLEGRO_KEY_RIGHT) {
					if(isPlaying){
						nextLocation = player.location;		
						if(nextLocation.x < size.x -1) nextLocation.x++;
						playerMovement(nextLocation);
					}else{
						navigateRight();
						drawMenu();
					}
					al_play_sample(setObject, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
				} 

				if(event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode  == ALLEGRO_KEY_UP) {
					if(isPlaying){
						nextLocation = player.location;
						if(nextLocation.y > 0) nextLocation.y--;
						playerMovement(nextLocation);
					}else{
						navigateUp();
						drawMenu();
					}
					al_play_sample(setObject, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
				} 

				if(event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode  == ALLEGRO_KEY_DOWN) {
					if(isPlaying){
						nextLocation = player.location;
						if(nextLocation.y < size.y -1) nextLocation.y++;
						playerMovement(nextLocation);
					}else{
						navigateDown();
						drawMenu();
					}	        	
					al_play_sample(setObject, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
				} 
				//pausing & entering
				if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
					if(!isPlaying){
						sound = al_load_sample( "../resources/sounds/set_flag.wav" );
						al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
						menuLogic(); 
						if(!isPlaying && !isEditing) drawMenu();
						else{
							if(isPlaying) drawGame();
							else drawEditor();
						}
					}
				}
			}

			if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
				if(isPlaying || isEditing){
					stopTileSound();
					al_play_sample(soundtrack, 0.75, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
					inEditorMenu = false;
					al_hide_mouse_cursor(window);
					if(isPlaying) templatePause();
					else {
						inEditorMenu = true;
						templatePauseEditor();
					}
					drawMenu();
					isPlaying = false;
					isEditing = false;
				}
			}

			//editor modes
			if(isEditing){
				al_show_mouse_cursor(window);
				if(event.keyboard.keycode == ALLEGRO_KEY_1){
					currentMode = 1;
					drawEditorGui();
				}
				if(event.keyboard.keycode == ALLEGRO_KEY_2){
					currentMode = 2;
					drawEditorGui();
				}
				if(event.keyboard.keycode == ALLEGRO_KEY_3){
					currentMode = 3;
					drawEditorGui();
				}
				if(event.keyboard.keycode == ALLEGRO_KEY_4){
					currentMode = 4;
					drawEditorGui();
				}
				if(event.keyboard.keycode == ALLEGRO_KEY_5){
					currentMode = 5;
					drawEditorGui();
				}
				if(event.keyboard.keycode == ALLEGRO_KEY_6){
					currentMode = 6;
					drawEditorGui();
				}
				if(event.keyboard.keycode == ALLEGRO_KEY_7){
					currentMode = 7;
					drawEditorGui();
				}
			}
		}

		if( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			if(isEditing){
				Coords clickedTile, start, end;
				int horizontalMargin = (al_get_display_width(window) > size.x * pixels + (size.x - 1) * gap)?((al_get_display_width(window) - (size.x * pixels + (size.x - 1) * gap))/2):(0);
				int x = event.mouse.x; 
				int y = event.mouse.y;
				
				for(int i = 0; i < size.x * size.y; i++){
					start.x = map[i].location.x * pixels + gap + horizontalMargin;
					start.y = map[i].location.y * pixels + gap + 100;
					end.x = map[i].location.x * pixels + pixels + horizontalMargin;
					end.y = map[i].location.y * pixels + pixels + 100;
					
					if(x >= start.x && x <= end.x){
						if(y >= start.y && y <= end.y){
							clickedTile.x = map[i].location.x;
							clickedTile.y = map[i].location.y;
							al_play_sample(setObject, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							break;
						} else i += size.x - 1;
					}
				}
				
				switch(currentMode){
					case 1:
						setEmpty(clickedTile);
						break;
					case 2:
						setBomb(clickedTile);
						break;
					case 3:
						setSpawner(clickedTile);
						break;
					case 4:
						setWall(clickedTile);
						break;
					case 5:
						setWater(clickedTile);
						break;
					case 6:
						setFreeSpace(clickedTile);
						break;
					case 7:
						setRandom(clickedTile);
						break;
					default:
						break;
				}
				drawEditor();
			}
		}

		if(event.type == ALLEGRO_EVENT_TIMER) {
			draw = true;
			if(isPlaying) gameTime += 1.0 ;
		}

		if(draw){
			draw = false;
			if(isPlaying == true) drawGame();
			else if(isEditing == true) drawEditor();
			else drawMenu();
		}
	}

	al_destroy_display(window);
	al_destroy_event_queue(eventQueue);
	destroyMap();
	destroyMenu();
	return 0;
}

void drawMenu(){
	al_clear_to_color(al_map_rgb(255, 243, 224));

	int displayHeight = al_get_display_height(window);
	int displayWidth = al_get_display_width(window);
	int horizontalMargin = displayWidth / 2;
	int verticalMargin = al_get_font_line_height(titleFont) + 50 + 100;
	int padding = 20;
	int previousWidth = 0;

	al_draw_text(titleFont, titleColor, horizontalMargin, 50, ALLEGRO_ALIGN_CENTRE, menuTitle.c_str());
	if(isHorizontal) horizontalMargin -= 50 * (rowElementsCount - 1);
	for(int i = 0; i < elementsCount; i++){
		previousWidth = 100;
		al_draw_text((al_get_display_height(window) < 900)?(menu[i].smallerFont):(menu[i].font), (menu[i].hover)?(menu[i].hoverColor):(menu[i].mainColor), menu[i].location.x * previousWidth + horizontalMargin, menu[i].location.y * (al_get_font_line_height((al_get_display_height(window) < 900)?(menu[i].smallerFont):(menu[i].font)) + padding) + verticalMargin, ALLEGRO_ALIGN_CENTRE, menu[i].text.c_str());
	}
	al_flip_display();
}

void drawGame(){
	al_clear_to_color(al_map_rgb(255, 243, 224));

	int horizontalGap, verticalGap;
	int displayWidth = al_get_display_width(window);
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
		if(map[i].flag == 3 && i != playerPos) al_draw_text(mapColors.questionMarkFont , mapColors.questionMarkFontColor ,(start.x + end.x) / 2, start.y + 6, ALLEGRO_ALIGN_CENTRE, "?");
		if(map[i].location.x == player.location.x && map[i].location.y == player.location.y){
			al_draw_filled_rectangle(start.x + (pixels/6), start.y + (pixels/6), end.x - (pixels/6), end.y - (pixels/6), player.color);
			al_draw_filled_rectangle(start.x + (pixels/3), start.y + (pixels/3), end.x - (pixels/3), end.y - (pixels/3), player.equipmentColor);
		}
	}
	drawGameGui();
}

void drawGameGui(){
	int displayWidth = al_get_display_width(window);
	bool small = (displayWidth < 900)?(true):(false);
	string tempText = "Flags: " + to_string(player.flagsEquipped);
	setText(0, tempText);
	int minutes = gameTime / 60, seconds = gameTime - (minutes * 60);
	string zero = (seconds < 10)?("0"):("");
	tempText = to_string(minutes) + ":"+ zero + to_string(seconds);
	setText(1, tempText);

	al_draw_filled_rectangle(0, 0, displayWidth, 100, al_map_rgb(255, 243, 224));
	al_draw_text((small)?(menu[0].smallerFont):(menu[0].font), menu[0].mainColor, 40, menu[0].location.y * (al_get_font_line_height((small)?(menu[0].smallerFont):(menu[0].font))) + 20, ALLEGRO_ALIGN_LEFT, menu[0].text.c_str());

	al_draw_text((small)?(menu[1].smallerFont):(menu[1].font), menu[1].mainColor, displayWidth / 2 , menu[1].location.y * (al_get_font_line_height((small)?(menu[1].smallerFont):(menu[1].font))) + 20, ALLEGRO_ALIGN_CENTRE, menu[1].text.c_str());

	al_draw_text((small)?(menu[2].smallerFont):(menu[2].font), menu[2].mainColor, displayWidth  - 40, menu[2].location.y * (al_get_font_line_height((small)?(menu[2].smallerFont):(menu[2].font))) + 20, ALLEGRO_ALIGN_RIGHT, menu[2].text.c_str());

	al_flip_display();
}

void drawEditor(){
	al_clear_to_color(al_map_rgb(255, 243, 224));

	int horizontalGap, verticalGap;
	int displayWidth = al_get_display_width(window);
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
		if(map[i].type == -5 && i != playerPos) al_draw_text(mapColors.questionMarkFont , mapColors.questionMarkFontColor ,(start.x + end.x) / 2, start.y + 6, ALLEGRO_ALIGN_CENTRE, "?");
	}
	drawEditorGui();
}

void drawEditorGui(){
	ALLEGRO_COLOR currentColor;
	int displayWidth = al_get_display_width(window);

	switch(currentMode){
		case 1:
			currentColor = al_map_rgb(255, 204, 128);
			break;
		case 2:
			currentColor = al_map_rgb(33, 33, 33);
			break;
		case 3:
			currentColor = al_map_rgb(175, 180, 43);
			break;
		case 4:
			currentColor = al_map_rgb(97, 97, 97);
			break;
		case 5:
			currentColor = al_map_rgb(0, 151, 167);
			break;
		case 6:
			currentColor = al_map_rgb(255, 224, 178);
			break;
		case 7:
			currentColor = al_map_rgb(93, 64, 55);
			break;
		default:
			break;
	}

	al_draw_filled_rectangle(displayWidth / 2 - 35, 20, displayWidth / 2 + 35, 90, currentColor);
	if(currentMode == 7) al_draw_text(mapColors.questionMarkFont , mapColors.questionMarkFontColor ,displayWidth / 2, 35 + 6, ALLEGRO_ALIGN_CENTRE, "?");
	al_flip_display();
}

void menuLogic(){
	string nextAction = menu[hoverElement].nextAction;
	
	if(nextAction == "EXIT"){
		isExiting = true;
	}else if(nextAction == "NEW_LOAD_GAME"){
		templateNewLoadGame();
	}else if(nextAction == "NEW_LOAD_MAP"){
		templateNewLoadMap();	
	} else if(nextAction == "NEW_GAME"){
		templateNewGame();
	} else if(nextAction == "NEW_MAP"){
		size.x = 2;
		size.y = 2;
		templateNewMap();
	} else if(nextAction == "NEW_MAP_CREATE"){
		bombs = 0;
		setLevel(size.x, size.y, bombs);
		createMap();
		al_stop_sample(&soundtrackId);
		isEditing = true;
	} else if(nextAction == "SAVE_MAP_EDITOR"){
		templateBombsQuantity();
		setText(4, to_string(bombsCount));
	} else if(nextAction == "PAUSE_EDITOR"){
		bombsCount = getPlacedBombsCount();
		templatePauseEditor();
	} else if(nextAction == "LOAD_GAME"){
		loadGame("");
	} else if(nextAction == "LOAD_MAP"){
		if(!loadMap(window, "")){
			al_show_native_message_box(window, "Error", "Error #9", "Failed to load the map!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		} else { 
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			openAll();
			setEditorColors();
			isEditing = true;
			al_stop_sample(&soundtrackId);
		}
	} else if(nextAction == "MAIN"){
		templateMain();
	} else if(nextAction == "ARCADE"){
		templateArcade();
	} else if(nextAction == "RETRO"){
		templateRetro();
	} else if(nextAction == "1"){
		loadGame("../resources/levels/1.map");
	} else if(nextAction == "2"){
		loadGame("../resources/levels/2.map");
	} else if(nextAction == "3"){
		loadGame("../resources/levels/3.map");
	} else if(nextAction == "4"){
		loadGame("../resources/levels/4.map");
	} else if(nextAction == "5"){
		loadGame("../resources/levels/5.map");
	} else if(nextAction == "6"){
		loadGame("../resources/levels/6.map");
	} else if(nextAction == "7"){
		loadGame("../resources/levels/7.map");
	} else if(nextAction == "8"){
		loadGame("../resources/levels/8.map");
	} else if(nextAction == "9"){
		loadGame("../resources/levels/9.map");
	} else if(nextAction == "10"){
		loadGame("../resources/levels/10.map");
	} else if(nextAction == "EASY"){
		size.x = 10;
		size.y = 10;
		bombs = 10;
		newGame();		
	} else if(nextAction == "MEDIUM"){
		size.x = 15;
		size.y = 10;
		bombs = 40;
		newGame();		
	} else if(nextAction == "HARD"){
		size.x = 20;
		size.y = 20;
		bombs = 150;
		newGame();
		playTileSound();		
	} else if(nextAction == "CUSTOM"){
		size.x = 2;
		size.y = 2;
		bombs = 1;
		templateCustom();
	} else if(nextAction == "SUBSTRACT_SIZE_X"){
		if(size.x > 2) size.x--;
		setText(4, to_string(size.x));		
	} else if(nextAction == "SUBSTRACT_SIZE_Y"){
		if(size.y > 2) size.y--;	
		setText(10, to_string(size.y));		
	} else if(nextAction == "SUBSTRACT_BOMBS"){
		if(inEditorMenu){
			if(bombsCount > getPlacedBombsCount()) bombsCount--;
			setText(4, to_string(bombsCount));
		} else {
			if(bombs > 1) bombs--;	
			setText(16, to_string(bombs));	
		}	
	} else if(nextAction == "ADD_SIZE_X"){
		if(size.x < 100) size.x++;	
		setText(4, to_string(size.x));		
	} else if(nextAction == "ADD_SIZE_Y"){
		if(size.y < 100) size.y++;
		setText(10, to_string(size.y));		
	} else if(nextAction == "ADD_BOMBS"){
		if(inEditorMenu){
			if(bombsCount < getPlacedBombsCount() + getRandomTilesCount() - 1) bombsCount++;
			setText(4, to_string(bombsCount));
		} else {
			if(bombs < size.x * size.y - 2) bombs++;
			setText(16, to_string(bombs));		
		}
	} else if(nextAction == "START_CUSTOM"){
		newGame();			
	} else if(nextAction == "NEW_GAME"){
		templateNewGame();
	} else if(nextAction == "RESUME"){
		al_stop_sample(&soundtrackId);
		if(inEditorMenu) {
			isEditing = true;
			inEditorMenu = false;
		} else {
			isPlaying = true;
			templateGameGui();
			playTileSound();
		}
	} else if(nextAction == "SAVE_MAP"){
		if(!saveMap(window)){
			al_show_native_message_box(window, "Error", "Error #10", "Failed to save the map!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		}else if(inEditorMenu) templateMain();
	}
}

void playTileSound(){
	int currentType = map[getTileFromLocation(player.location)].type;
	if(map[getTileFromLocation(player.location)].flag <= 1 && previousType != currentType){
		al_stop_sample(&tileSoundId);
		switch(currentType){
			case 0:
				sound = al_load_sample( "../resources/sounds/noise.wav" );
				break;
			case 1:
				sound = al_load_sample( "../resources/sounds/near_mine_1.wav" );
				break;
			case 2:
				sound = al_load_sample( "../resources/sounds/near_mine_2.wav" );
				break;
			case 3:
				sound = al_load_sample( "../resources/sounds/near_mine_3.wav" );
				break;
			case 4:
				sound = al_load_sample( "../resources/sounds/near_mine_4.wav" );
				break;
			case 5:
				sound = al_load_sample( "../resources/sounds/near_mine_5.wav" );
				break;
			case 6:
				sound = al_load_sample( "../resources/sounds/near_mine_6.wav" );
				break;
			case 7:
				sound = al_load_sample( "../resources/sounds/near_mine_7.wav" );
				break;
			case 8:
				sound = al_load_sample( "../resources/sounds/near_mine_8.wav" );
				break;
			default:
				break;
		}
		al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &tileSoundId);
		previousType = currentType;
	}
	return;
}

void stopTileSound(){
	al_stop_sample(&tileSoundId);
	return;
}

void playerMovement(Coords nextLocation){
	if(setFlag == 0 && canMove(nextLocation)) {
		move(nextLocation);
		playTileSound();
	} else if(setFlag == 2) {
		int state = toggleTileFlag(nextLocation, setFlag);
		if(state == 1) {
			if(hasFlags()){
				putFlag();
				sound = al_load_sample( "../resources/sounds/set_flag.wav" );
				al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			} else toggleTileFlag(nextLocation, setFlag);
		} else if(state == 2){
			sound = al_load_sample( "../resources/sounds/take_flag.wav" );
			al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			takeFlag();
		} 
	} else {
		int state = toggleTileFlag(nextLocation, setFlag);
			if(state == 3){ 
				sound = al_load_sample( "../resources/sounds/set_flag.wav" );
				al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			} else if(state == 2) {
				sound = al_load_sample( "../resources/sounds/take_flag.wav" );
				al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			} 
	}
						
	if(!openTile(player.location)){
		drawGame();
		explosion();
		templateStatus(false);
		al_rest(2.0);
		isPlaying = false;
		drawMenu();
		al_play_sample(soundtrack, 0.75, 0.0,1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
	}else if(win()){
		drawGame();
		al_rest(0.5);
		crown();
		templateStatus(true);
		al_rest(3.0);
		isPlaying = false;
		drawMenu();
		al_play_sample(soundtrack, 0.75, 0.0,1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
	}else drawGame();
}

void startPlaying(){
	spawnPlayer(getLocationFromTile(playerPos), bombs);
	al_stop_sample(&soundtrackId);
	gameTime = 0.0;
	isPlaying = true;
	templateGameGui();
	playTileSound();
}

void loadGame(string map){
	if(!loadMap(window, map)){
		al_show_native_message_box(window, "Error", "Error #9", "Failed to load the map!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}else{
		size.x = sizeX;
		size.y = sizeY;
		bombs = bombsCount;
		fillRandomTiles();
		closeAll();
		startPlaying();
	}
}

void newGame(){
	setLevel(size.x, size.y, bombs);
	if(!generateMap()) al_show_native_message_box(window, "Error", "Error #9", "Failed to load the map!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	else startPlaying();

}

void explosion(){
	Coords start, end;
	Coords initialLocation = player.location;
	int displayWidth = al_get_display_width(window);
	int horizontalMargin = (displayWidth > size.x * pixels + (size.x - 1) * gap)?((displayWidth - (size.x * pixels + (size.x - 1) * gap))/2):(0);
	int verticalMargin = 100;

	stopTileSound();
	sound = al_load_sample( "../resources/sounds/explosion.wav" );
	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
	ALLEGRO_COLOR smokeColor, smokeBorderColor;

	for(int i = 0; i < 60; i++){

		switch(i){
			case 0:
				smokeColor = al_map_rgb(255, 61, 0);
				smokeBorderColor = al_map_rgb(0, 0, 0);
				break;
			case 5:
				smokeColor = al_map_rgb(33, 33, 33);
				break;
			case 15:
				smokeColor = al_map_rgb(66, 66, 66);
				break;
			case 25:
				smokeColor = al_map_rgb(97, 97, 97);
				break;
			case 40:
				smokeColor = al_map_rgb(189, 189, 189);
				break;
			case 58:
				smokeColor = al_map_rgb(224, 224, 224);
				break;
			default:
				break;
		}

		int randomX = rand() % (pixels) + 5 * i;
		int randomY = (rand() % (pixels) + 3 * i) * (rand() % 2 * -1);
		int smokeSize = rand() % pixels + 20;

		start.x = initialLocation.x * pixels + horizontalMargin + randomX;
		start.y = initialLocation.y * pixels + verticalMargin + randomY;
		end.x = initialLocation.x * pixels + smokeSize + horizontalMargin + randomX;
		end.y = initialLocation.y * pixels + smokeSize + verticalMargin + randomY;
		if(start.y > 100 && end.y > 100){
			al_draw_filled_rectangle(start.x, start.y, end.x, end.y, smokeColor);
			al_draw_rectangle(start.x, start.y, end.x, end.y, smokeBorderColor, 2);
		}

		randomX *= -1;
		randomY = (rand()% (pixels) + 3 * i) * (rand() % 2 * -1);
		smokeSize = rand() % pixels + 10;

		start.x = initialLocation.x * pixels + horizontalMargin + randomX;
		start.y = initialLocation.y * pixels + verticalMargin + randomY;
		end.x = initialLocation.x * pixels + smokeSize + horizontalMargin + randomX;
		end.y = initialLocation.y * pixels + smokeSize + verticalMargin + randomY;
		if(start.y > 100 && end.y > 100){
			al_draw_filled_rectangle(start.x, start.y, end.x, end.y, smokeColor);
			al_draw_rectangle(start.x, start.y, end.x, end.y, smokeBorderColor, 2);
		}

		randomX = (rand()% (pixels) + 3 * i) * (rand() % 2 * -1);
		randomY = rand()% (pixels) + 5 * i;
		smokeSize = rand() % pixels + 10;

		start.x = initialLocation.x * pixels + horizontalMargin + randomX;
		start.y = initialLocation.y * pixels + verticalMargin + randomY;
		end.x = initialLocation.x * pixels + smokeSize + horizontalMargin + randomX;
		end.y = initialLocation.y * pixels + smokeSize + verticalMargin + randomY;
		if(start.y > 100 && end.y > 100){
			al_draw_filled_rectangle(start.x, start.y, end.x, end.y, smokeColor);
			al_draw_rectangle(start.x, start.y, end.x, end.y, smokeBorderColor, 2);
		}

		randomX = (rand()% (pixels) + 3 * i)* (rand() % 2 * -1);
		randomY *= -1;
		smokeSize = rand() % pixels + 10;

		start.x = initialLocation.x * pixels + horizontalMargin + randomX;
		start.y = initialLocation.y * pixels + verticalMargin + randomY;
		end.x = initialLocation.x * pixels + smokeSize + horizontalMargin + randomX;
		end.y = initialLocation.y * pixels + smokeSize + verticalMargin + randomY;
		if(start.y > 100 && end.y > 100){
			al_draw_filled_rectangle(start.x, start.y, end.x, end.y, smokeColor);
			al_draw_rectangle(start.x, start.y, end.x, end.y, smokeBorderColor, 2);
		}
		al_flip_display();
		sleep(0.1);
	}
}

void crown(){
	stopTileSound();
	sound = al_load_sample( "../resources/sounds/win.wav" );
	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

	int displayWidth = al_get_display_width(window);
	int displayHeight = al_get_display_height(window);
	Coords start, end;

	start.x = displayWidth / 2 - 300;
	start.y = displayHeight / 2 - 250;
	end.x = displayWidth / 2 + 300;
	end.y = displayHeight / 2 + 150;

	al_draw_filled_rounded_rectangle(start.x, start.y, end.x, end.y, 10, 10, al_map_rgb(255, 229, 127));
	al_draw_rounded_rectangle(start.x, start.y, end.x, end.y, 10, 10, al_map_rgb(253, 216, 53), 10);

	al_draw_filled_triangle(start.x + 50, end.y - 50, displayWidth / 2, start.y + 50, end.x - 50, end.y - 50, al_map_rgb(253, 216, 53));
	al_draw_filled_triangle(start.x + 50, end.y - 50, start.x + 50, start.y + 50, displayWidth / 2, end.y - 50, al_map_rgb(251, 192, 45));
	al_draw_filled_triangle(displayWidth / 2, end.y - 50, end.x - 50, start.y + 50, end.x - 50, end.y - 50, al_map_rgb(249, 168, 37));
	for(int i = 0; i < 3; i++){
	al_draw_filled_circle(displayWidth / 2, end.y - 120, 20, al_map_rgb(240, 98, 146));
	al_draw_filled_circle(displayWidth / 2, start.y + 150, 15, al_map_rgb(240, 98, 146));
	al_draw_filled_circle(start.x + 100, end.y - 100, 15, al_map_rgb(236, 64, 122));
	al_draw_filled_circle(end.x - 100, end.y - 100, 15, al_map_rgb(236, 64, 122));
	al_flip_display();
	al_rest(0.1);
	al_draw_filled_circle(displayWidth / 2, end.y - 120, 20, al_map_rgb(186, 104, 200));
	al_draw_filled_circle(displayWidth / 2, start.y + 150, 15, al_map_rgb(186, 104, 200));
	al_draw_filled_circle(start.x + 100, end.y - 100, 15, al_map_rgb(171, 71, 188));
	al_draw_filled_circle(end.x - 100, end.y - 100, 15, al_map_rgb(171, 71, 188));
	al_flip_display();
	al_rest(0.25);
	}
}