#include "../headers/main.h"
#include "../headers/player.h"
#include "../headers/map.h"
#include "../headers/gui.h"

bool isExiting = false, isPlaying = false, isEditing = false, inEditor = false;
Coords size = {2, 2};
int bombs = 1; 
int pixels = 50;
int gap = 5;
double gameTime = 0.0;
short int currentMode = 1;
ALLEGRO_DISPLAY *window = NULL;
ALLEGRO_SAMPLE *soundtrack = NULL;
ALLEGRO_SAMPLE *setObject = NULL;
ALLEGRO_SAMPLE *sound = NULL;
ALLEGRO_SAMPLE *tileSound = NULL;
ALLEGRO_SAMPLE_ID soundtrackId, tileSoundId;

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

	if(!al_install_mouse()){
    	cout << "failed to initialize mouse!\n";
    	return -1;
    }

    if(!al_init_primitives_addon()){
       	cout << "failed to initialize primitives!\n";
    	return -1;
    }

    if(!al_install_audio()){
      cout << "failed to initialize audio!\n";
      return -1;
   }

   if(!al_init_acodec_addon()){
      cout << "failed to initialize audio codecs!\n";
      return -1;
   }
 
   if (!al_reserve_samples(4)){
      cout << "failed to reserve samples!\n";
      return -1;
   }
 
   soundtrack = al_load_sample( "../resources/sounds/background.wav" );
   setObject = al_load_sample( "../resources/sounds/set_item.wav" );
 
   if (!soundtrack){
      cout << "Audio clip sample not loaded!\n"; 
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
    	cout << "failed to create display!\n";
    	return -1;
  	}
  	  	
  	templateMain();
  	drawMenu(al_get_display_height(window), al_get_display_width(window));
  	al_play_sample(soundtrack, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &soundtrackId);
    al_hide_mouse_cursor(window);
  	Coords nextLocation;
  	short int setFlag = 0;
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
		       			drawGame(al_get_display_width(window));
		       			al_play_sample(setObject, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		       		}
	  			}

	  			if(event.keyboard.keycode == ALLEGRO_KEY_F){ 
	  				if(isPlaying){
	  					setFlag = (setFlag == 0)?(2):(0);
	  					player.equipmentColor = (setFlag == 2)?(al_map_rgb(183, 28, 28)):(al_map_rgb(175, 180, 43));
	  					drawGame(al_get_display_width(window));
	  					al_play_sample(setObject, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
	  				}
				}

				//moving
		  		if(event.keyboard.keycode  == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
		  			if(isPlaying){
		  				nextLocation = player.location;
			  			
			  			if(nextLocation.x > 0) nextLocation.x--;
			  			if(setFlag == 0 && canMove(nextLocation)) {
			  				move(nextLocation);
			  				playTileSound();
			  			}
			  			else if(setFlag == 2) {
			  				int state = toggleTileFlag(nextLocation, setFlag);
			  				if( state == 1) {
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
		       				stopTileSound();
		       				sound = al_load_sample( "../resources/sounds/explosion.wav" );
		        			al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		       				openAll();
		       				drawGame(al_get_display_width(window));
		       				templateStatus(false);
		       				al_rest(2.0);
		       				isPlaying = false;
		       				drawMenu(al_get_display_height(window), al_get_display_width(window));
		       				al_play_sample(soundtrack, 0.5, 0.0,1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
		       			}else if(win()){
		       				stopTileSound();
		       				sound = al_load_sample( "../resources/sounds/win.wav" );
		        			al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		       				drawGame(al_get_display_width(window));
		       				templateStatus(true);
		       				al_rest(2.0);
		       				isPlaying = false;
		       				drawMenu(al_get_display_height(window), al_get_display_width(window));
		       				al_play_sample(soundtrack, 0.5, 0.0,1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
						}else{
		       				drawGame(al_get_display_width(window));
						}
		  			}else{
		  				navigateLeft();
	       				drawMenu(al_get_display_height(window), al_get_display_width(window));
		  			}
		  			al_play_sample(setObject, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		        } 

		        if(event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode  == ALLEGRO_KEY_RIGHT) {
		  			if(isPlaying){
		  				nextLocation = player.location;
			  			
			  			if(nextLocation.x < size.x -1) nextLocation.x++;
			  			if(setFlag == 0 && canMove(nextLocation)) {
			  				move(nextLocation);
			  				playTileSound();
			  			}
			  			else if(setFlag == 2) {
			  				int state = toggleTileFlag(nextLocation, setFlag);
			  				if( state == 1) {
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
		       				stopTileSound();
		       				sound = al_load_sample( "../resources/sounds/explosion.wav" );
		        			al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		       				openAll();
		       				drawGame(al_get_display_width(window));
		       				templateStatus(false);
		       				al_rest(2.0);
		       				isPlaying = false;
	       					drawMenu(al_get_display_height(window), al_get_display_width(window));
	       					al_play_sample(soundtrack, 0.5, 0.0,1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
		       			}else if(win()){
		       				stopTileSound();
		       				sound = al_load_sample( "../resources/sounds/win.wav" );
		        			al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		       				drawGame(al_get_display_width(window));
		       				templateStatus(true);
		       				al_rest(2.0);
		       				isPlaying = false;
	      	 				drawMenu(al_get_display_height(window), al_get_display_width(window));
	      	 				al_play_sample(soundtrack, 0.5, 0.0,1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
	      	 			}else{
		       				drawGame(al_get_display_width(window));
		       			}
		  			}else{
		  				navigateRight();
	      	 			drawMenu(al_get_display_height(window), al_get_display_width(window));
		  			}
		  			al_play_sample(setObject, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		        } 

		        if(event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode  == ALLEGRO_KEY_UP) {
		  			if(isPlaying){
		  				nextLocation = player.location;
			  			
			  			if(nextLocation.y > 0) nextLocation.y--;
			  			if(setFlag == 0 && canMove(nextLocation)) {
			  				move(nextLocation);
			  				playTileSound();
			  			}
			  			else if(setFlag == 2) {
			  				int state = toggleTileFlag(nextLocation, setFlag);
			  				if( state == 1) {
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
		       				stopTileSound();
		       				sound = al_load_sample( "../resources/sounds/explosion.wav" );
		        			al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		       				openAll();
		       				drawGame(al_get_display_width(window));
		       				templateStatus(false);
		       				al_rest(2.0);
		       				isPlaying = false;
		       				drawMenu(al_get_display_height(window), al_get_display_width(window));
		       				al_play_sample(soundtrack, 0.5, 0.0,1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
		       			}else if(win()){
		       				stopTileSound();
		       				sound = al_load_sample( "../resources/sounds/win.wav" );
		        			al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		       				drawGame(al_get_display_width(window));
		       				templateStatus(true);
		       				al_rest(2.0);
		       				isPlaying = false;
		       				drawMenu(al_get_display_height(window), al_get_display_width(window));
		       				al_play_sample(soundtrack, 0.5, 0.0,1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
		       			}else{
		       				drawGame(al_get_display_width(window));
						}
		  			}else{
		  				navigateUp();
		  				drawMenu(al_get_display_height(window), al_get_display_width(window));
		  			}
		  			al_play_sample(setObject, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		        } 

		        if(event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode  == ALLEGRO_KEY_DOWN) {
		  			if(isPlaying){
		  				nextLocation = player.location;
			  			
			  			if(nextLocation.y < size.y -1) nextLocation.y++;
			  			if(setFlag == 0 && canMove(nextLocation)) {
			  				move(nextLocation);
			  				playTileSound();
			  			}
			  			else if(setFlag == 2) {
			  				int state = toggleTileFlag(nextLocation, setFlag);
			  				if( state == 1) {
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
		       				stopTileSound();
		       				sound = al_load_sample( "../resources/sounds/explosion.wav" );
		        			al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		       				openAll();
		       				drawGame(al_get_display_width(window));
		       				templateStatus(false);
		       				al_rest(2.0);
		       				isPlaying = false;
		       				drawMenu(al_get_display_height(window), al_get_display_width(window));
		       				al_play_sample(soundtrack, 0.5, 0.0,1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
		       			}else if(win()){
		       				stopTileSound();
		       				sound = al_load_sample( "../resources/sounds/win.wav" );
		        			al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		       				drawGame(al_get_display_width(window));
		       				templateStatus(true);
		       				al_rest(2.0);
		       				isPlaying = false;
		       				drawMenu(al_get_display_height(window), al_get_display_width(window));
		       				al_play_sample(soundtrack, 0.5, 0.0,1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
		       			}else{
		       				drawGame(al_get_display_width(window));
						}
		  			}else{
		  				navigateDown();
		  				drawMenu(al_get_display_height(window), al_get_display_width(window));
		  			}	        	
		  			al_play_sample(setObject, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		        } 
		        //pausing & entering
		        if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
		        	if(!isPlaying){
		        		sound = al_load_sample( "../resources/sounds/set_flag.wav" );
		        		al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		        		menuLogic(); 
		        		if(!isPlaying && !isEditing) drawMenu(al_get_display_height(window), al_get_display_width(window));
		        		else{
		        			 if(isPlaying) drawGame(al_get_display_width(window));
		        			else drawEditor(al_get_display_width(window));
		        		}
		        	}
		        }
	    	}

	        if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
	        	if(isPlaying || isEditing){
	        		stopTileSound();
  					al_play_sample(soundtrack, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP ,&soundtrackId);
	        		inEditor = false;
                    al_hide_mouse_cursor(window);
	        		if(isPlaying) templatePause();
	        		else {
	        			inEditor = true;
	        			templatePauseEditor();
	        		}
	        		drawMenu(al_get_display_height(window), al_get_display_width(window));
	        		isPlaying = false;
	        		isEditing = false;
	        	}
	        }

	        //editor modes
	        if(isEditing){
                al_show_mouse_cursor(window);
	        	if(event.keyboard.keycode == ALLEGRO_KEY_1){
		        	currentMode = 1;
		        	drawEditor(al_get_display_width(window));
	       		}
	       		if(event.keyboard.keycode == ALLEGRO_KEY_2){
		        	currentMode = 2;
		        	drawEditor(al_get_display_width(window));
	       		}
	       		if(event.keyboard.keycode == ALLEGRO_KEY_3){
		        	currentMode = 3;
		        	drawEditor(al_get_display_width(window));
	       		}
	       		if(event.keyboard.keycode == ALLEGRO_KEY_4){
		        	currentMode = 4;
		        	drawEditor(al_get_display_width(window));
	       		}
	       		if(event.keyboard.keycode == ALLEGRO_KEY_5){
		        	currentMode = 5;
		        	drawEditor(al_get_display_width(window));
	       		}
	       		if(event.keyboard.keycode == ALLEGRO_KEY_6){
		        	currentMode = 6;
		        	drawEditor(al_get_display_width(window));
	       		}
	       		if(event.keyboard.keycode == ALLEGRO_KEY_7){
		        	currentMode = 7;
		        	drawEditor(al_get_display_width(window));
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
	    					al_play_sample(setObject, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
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
    			drawEditor(al_get_display_width(window));
    		}
    	}

  	if(event.type == ALLEGRO_EVENT_TIMER) {
  		   //draw = true;
  		   if(isPlaying) gameTime += 1.0 ;
    }

    if(draw){
    	draw = false;
    	if(isPlaying) drawGame(al_get_display_width(window));
        else if(isEditing) drawEditor(al_get_display_width(window));
        else drawMenu(al_get_display_height(window), al_get_display_width(window));
    }
  	}

    al_destroy_display(window);
    al_destroy_event_queue(eventQueue);
    destroyMap();
    destroyMenu();
    return 0;
}

void drawGame(int displayWidth){
	al_clear_to_color(al_map_rgb(255, 243, 224));
	templateGameGui();
	string tempText = "Flags: " + to_string(player.flagsEquipped);
	setText(0, tempText);
	int minutes = gameTime / 60, seconds = gameTime - minutes;
	string zero = (seconds < 10)?("0"):("");
	tempText = to_string(minutes) + ":"+ zero + to_string(seconds);
	setText(1, tempText);
	int horizontalGap, verticalGap;
	Coords start, end;
	int horizontalMargin = (displayWidth > size.x * pixels + (size.x - 1) * gap)?((displayWidth - (size.x * pixels + (size.x - 1) * gap))/2):(0);
	int verticalMargin = 100;

		al_draw_text((al_get_display_height(window) < 900)?(menu[0].smallerFont):(menu[0].font), menu[0].mainColor, 40, menu[0].location.y * (al_get_font_line_height((al_get_display_height(window) < 900)?(menu[0].smallerFont):(menu[0].font))) + 20, ALLEGRO_ALIGN_LEFT, menu[0].text.c_str());

		al_draw_text((al_get_display_height(window) < 900)?(menu[1].smallerFont):(menu[1].font), menu[1].mainColor, displayWidth / 2 , menu[1].location.y * (al_get_font_line_height((al_get_display_height(window) < 900)?(menu[1].smallerFont):(menu[1].font))) + 20, ALLEGRO_ALIGN_CENTRE, menu[1].text.c_str());

		al_draw_text((al_get_display_height(window) < 900)?(menu[2].smallerFont):(menu[2].font), menu[2].mainColor, displayWidth  - 40, menu[2].location.y * (al_get_font_line_height((al_get_display_height(window) < 900)?(menu[2].smallerFont):(menu[2].font))) + 20, ALLEGRO_ALIGN_RIGHT, menu[2].text.c_str());

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
		al_draw_text((al_get_display_height(window) < 900)?(menu[i].smallerFont):(menu[i].font), (menu[i].hover)?(menu[i].hoverColor):(menu[i].mainColor), menu[i].location.x * previousWidth + horizontalMargin, menu[i].location.y * (al_get_font_line_height((al_get_display_height(window) < 900)?(menu[i].smallerFont):(menu[i].font)) + padding) + verticalMargin, ALLEGRO_ALIGN_CENTRE, menu[i].text.c_str());
	}
	al_flip_display();
}

void drawEditor(int displayWidth){
	al_clear_to_color(al_map_rgb(255, 243, 224));
	int horizontalGap, verticalGap;
	Coords start, end;
	int horizontalMargin = (displayWidth > size.x * pixels + (size.x - 1) * gap)?((displayWidth - (size.x * pixels + (size.x - 1) * gap))/2):(0);
	int verticalMargin = 100;
	ALLEGRO_COLOR currentColor;
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
	al_flip_display();
}

void menuLogic(){
	if(menu[hoverElement].nextAction == "EXIT"){
		isExiting = true;
	}else if(menu[hoverElement].nextAction == "NEW_LOAD_GAME"){
		templateNewLoadGame();
	}else if(menu[hoverElement].nextAction == "NEW_LOAD_MAP"){
		templateNewLoadMap();	
	} else if(menu[hoverElement].nextAction == "NEW_GAME"){
		templateNewGame();
	} else if(menu[hoverElement].nextAction == "NEW_MAP"){
		size.x = 2;
		size.y = 2;
		templateNewMap();
	} else if(menu[hoverElement].nextAction == "NEW_MAP_CREATE"){
		bombs = 0;
		setLevel(size.x, size.y, bombs);
  		createMap();
  		al_stop_sample(&soundtrackId);
  		isEditing = true;
  	} else if(menu[hoverElement].nextAction == "SAVE_MAP_EDITOR"){
  		templateBombsQuantity();
  		setText(4, to_string(bombsCount));
  	} else if(menu[hoverElement].nextAction == "PAUSE_EDITOR"){
  		bombsCount = getPlacedBombsCount();
  		templatePauseEditor();
	} else if(menu[hoverElement].nextAction == "LOAD_GAME"){
		if(!loadMap(window, "")){
			cout << "Coudn't load the map";
		}else{
		size.x = sizeX;
		size.y = sizeY;
		bombs = bombsCount;
		fillRandomTiles();
		closeAll();
		spawnPlayer(getLocationFromTile(playerPos), bombs);
		al_stop_sample(&soundtrackId);
		gameTime = 0.0;
		isPlaying = true;
		playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "LOAD_MAP"){
		//TODO EDITOR LOAD MAP
		if(!loadMap(window, "")){
			cout << "Coudn't load the map";
		} else { 
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			openAll();
			setEditorColors();
			isEditing = true;
			al_stop_sample(&soundtrackId);
		}
	} else if(menu[hoverElement].nextAction == "MAIN"){
		templateMain();
	} else if(menu[hoverElement].nextAction == "ARCADE"){
		templateArcade();
	} else if(menu[hoverElement].nextAction == "RETRO"){
		templateRetro();
	} else if(menu[hoverElement].nextAction == "1"){
		if(!loadMap(window, "../resources/levels/1.map")){
			cout << "Coudn't load the map";
		}else{
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			fillRandomTiles();
			closeAll();
			spawnPlayer(getLocationFromTile(playerPos), bombs);
			al_stop_sample(&soundtrackId);
			gameTime = 0.0;
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "2"){
		if(!loadMap(window, "../resources/levels/2.map")){
			cout << "Coudn't load the map";
		}else{
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			fillRandomTiles();
			closeAll();
			spawnPlayer(getLocationFromTile(playerPos), bombs);
			al_stop_sample(&soundtrackId);
			gameTime = 0.0;
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "3"){
		if(!loadMap(window, "../resources/levels/3.map")){
			cout << "Coudn't load the map";
		}else{
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			fillRandomTiles();
			closeAll();
			spawnPlayer(getLocationFromTile(playerPos), bombs);
			al_stop_sample(&soundtrackId);
			gameTime = 0.0;
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "4"){
		if(!loadMap(window, "../resources/levels/4.map")){
			cout << "Coudn't load the map";
		}else{
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			fillRandomTiles();
			closeAll();
			spawnPlayer(getLocationFromTile(playerPos), bombs);
			al_stop_sample(&soundtrackId);
			gameTime = 0.0;
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "5"){
		if(!loadMap(window, "../resources/levels/5.map")){
			cout << "Coudn't load the map";
		}else{
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			fillRandomTiles();
			closeAll();
			spawnPlayer(getLocationFromTile(playerPos), bombs);
			al_stop_sample(&soundtrackId);
			gameTime = 0.0;
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "6"){
		if(!loadMap(window, "../resources/levels/6.map")){
			cout << "Coudn't load the map";
		}else{
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			fillRandomTiles();
			closeAll();
			spawnPlayer(getLocationFromTile(playerPos), bombs);
			al_stop_sample(&soundtrackId);
			gameTime = 0.0;
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "7"){
		if(!loadMap(window, "../resources/levels/7.map")){
			cout << "Coudn't load the map";
		}else{
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			fillRandomTiles();
			closeAll();
			spawnPlayer(getLocationFromTile(playerPos), bombs);
			al_stop_sample(&soundtrackId);
			gameTime = 0.0;
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "8"){
		if(!loadMap(window, "../resources/levels/8.map")){
			cout << "Coudn't load the map";
		}else{
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			fillRandomTiles();
			closeAll();
			spawnPlayer(getLocationFromTile(playerPos), bombs);
			al_stop_sample(&soundtrackId);
			gameTime = 0.0;
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "9"){
		if(!loadMap(window, "../resources/levels/9.map")){
			cout << "Coudn't load the map";
		}else{
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			fillRandomTiles();
			closeAll();
			spawnPlayer(getLocationFromTile(playerPos), bombs);
			al_stop_sample(&soundtrackId);
			gameTime = 0.0;
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "10"){
		if(!loadMap(window, "../resources/levels/10.map")){
			cout << "Coudn't load the map";
		}else{
			size.x = sizeX;
			size.y = sizeY;
			bombs = bombsCount;
			fillRandomTiles();
			closeAll();
			spawnPlayer(getLocationFromTile(playerPos), bombs);
			al_stop_sample(&soundtrackId);
			gameTime = 0.0;
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "EASY"){
		size.x = 10;
		size.y = 10;
		bombs = 10;
		setLevel(size.x, size.y, bombs);
  		if(!generateMap()){
			cout << "Coudn't generate the map";
		} else {
  		spawnPlayer(getLocationFromTile(playerPos), bombs);
  		al_stop_sample(&soundtrackId);
  		gameTime = 0.0;
		isPlaying = true;	
		playTileSound();		
		}		
	} else if(menu[hoverElement].nextAction == "MEDIUM"){
		size.x = 15;
		size.y = 10;
		bombs = 40;
		setLevel(size.x, size.y, bombs);
  		if(!generateMap()){
			cout << "Coudn't generate the map";
		} else {
  		spawnPlayer(getLocationFromTile(playerPos), bombs);
  		al_stop_sample(&soundtrackId);
  		gameTime = 0.0;
		isPlaying = true;	
		playTileSound();		
		}		
	} else if(menu[hoverElement].nextAction == "HARD"){
		size.x = 20;
		size.y = 20;
		bombs = 150;
		setLevel(size.x, size.y, bombs);
  		if(!generateMap()){
			cout << "Coudn't generate the map";
		} else {
  		spawnPlayer(getLocationFromTile(playerPos), bombs);
  		al_stop_sample(&soundtrackId);
  		gameTime = 0.0;
		isPlaying = true;
		playTileSound();			
		}			
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
		if(inEditor){
			if(bombsCount > getPlacedBombsCount()) bombsCount--;
			setText(4, to_string(bombsCount));
		}else{
		if(bombs > 1) bombs--;	
		setText(16, to_string(bombs));	
		}	
	} else if(menu[hoverElement].nextAction == "ADD_SIZE_X"){
		if(size.x < 100) size.x++;	
		setText(4, to_string(size.x));		
	} else if(menu[hoverElement].nextAction == "ADD_SIZE_Y"){
		if(size.y < 100) size.y++;
		setText(10, to_string(size.y));		
	} else if(menu[hoverElement].nextAction == "ADD_BOMBS"){
		if(inEditor){
			if(bombsCount < getPlacedBombsCount() + getRandomTilesCount() - 1) bombsCount++;
			setText(4, to_string(bombsCount));
		}else{
			if(bombs < size.x * size.y - 2) bombs++;
			setText(16, to_string(bombs));		
		}
	} else if(menu[hoverElement].nextAction == "START_CUSTOM"){
		setLevel(size.x, size.y, bombs);
  		if(!generateMap()){
			cout << "Coudn't generate the map";
		} else {
  		spawnPlayer(getLocationFromTile(playerPos), bombs);
  		al_stop_sample(&soundtrackId);
  		gameTime = 0.0;
		isPlaying = true;
		playTileSound();			
		}			
	} else if(menu[hoverElement].nextAction == "NEW_GAME"){
		templateNewGame();
	} else if(menu[hoverElement].nextAction == "RESUME"){
		al_stop_sample(&soundtrackId);
		if(inEditor) {
			isEditing = true;
			inEditor = false;
		}else {
			isPlaying = true;
			playTileSound();
		}
	} else if(menu[hoverElement].nextAction == "SAVE_MAP"){
		if(!saveMap(window)){
			cout << "Coudn't save the map";
		}else if(inEditor) templateMain();
	}
}

void playTileSound(){
	switch(map[getTileFromLocation(player.location)].type){
		case 0:
			al_stop_sample(&tileSoundId);
			sound = al_load_sample( "../resources/sounds/noise.wav" );
		  	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &tileSoundId);
			break;
		case 1:
			al_stop_sample(&tileSoundId);
			sound = al_load_sample( "../resources/sounds/near_mine_1.wav" );
		  	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &tileSoundId);
			break;
		case 2:
			al_stop_sample(&tileSoundId);
			sound = al_load_sample( "../resources/sounds/near_mine_2.wav" );
		  	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &tileSoundId);
			break;
		case 3:
			al_stop_sample(&tileSoundId);
			sound = al_load_sample( "../resources/sounds/near_mine_3.wav" );
		  	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &tileSoundId);
			break;
		case 4:
			al_stop_sample(&tileSoundId);
			sound = al_load_sample( "../resources/sounds/near_mine_4.wav" );
		  	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &tileSoundId);
			break;
		case 5:
			al_stop_sample(&tileSoundId);
			sound = al_load_sample( "../resources/sounds/near_mine_5.wav" );
		  	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &tileSoundId);
			break;
		case 6:
			al_stop_sample(&tileSoundId);
			sound = al_load_sample( "../resources/sounds/near_mine_6.wav" );
		  	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &tileSoundId);
			break;
		case 7:
			al_stop_sample(&tileSoundId);
			sound = al_load_sample( "../resources/sounds/near_mine_7.wav" );
		  	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &tileSoundId);
			break;
		case 8:
			al_stop_sample(&tileSoundId);
			sound = al_load_sample( "../resources/sounds/near_mine_8.wav" );
		  	al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &tileSoundId);
			break;
		default:
			break;
	}
	return;
}

void stopTileSound(){
	al_stop_sample(&tileSoundId);
	return;
}