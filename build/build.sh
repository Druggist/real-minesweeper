#!/bin/bash
g++ ../sources/main.cpp ../sources/map.cpp -o app.exe `pkg-config --libs allegro-5.0 allegro_main-5.0 allegro_primitives-5.0`
