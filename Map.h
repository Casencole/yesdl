//
// Created by casen on 2/10/2024.
//
#include <stdbool.h>
#include "SDL2/SDL.h"
#ifndef YESDL_MAP_H
#define YESDL_MAP_H
typedef struct{
    int rows;
    int cols;
    int avtrRow;
    int avtrCols;
    SDL_Rect** tile; 
} Map;

bool mapInit(Map* map, const char* filename);
void mapUninit(Map* map);
void displayMap(Map* map, SDL_Renderer* renderer);
void moveAvtr(Map* map, SDL_Renderer* renderer, char axis, int spaces);

#endif //YESDL_MAP_H
