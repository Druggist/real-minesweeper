#!/bin/bash
g++ -std=c++11 ../sources/main.cpp ../sources/map.cpp ../sources/player.cpp ../sources/gui.cpp -o app.exe `pkg-config --libs allegro-5.0 allegro_main-5.0 allegro_primitives-5.0 allegro_ttf-5.0 allegro_font-5.0 allegro_dialog-5.0`
