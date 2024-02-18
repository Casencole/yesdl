//
// Created by casen on 2/10/2024.
//
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "Avatar.h"
//#include "Texture.h"

#ifndef YESDL_MAP_H
#define YESDL_MAP_H

typedef enum {
  empty,
  wall, 
  fake_wall,
  gem, 
  key, 
  open_door, 
  locked_door,
  invalid_tile,
  wall_top
}tileName;

typedef struct {
    SDL_Rect rect;
    tileName type;
} Tile;

typedef struct{
    int rows;
    int cols;
    int avtrRow;
    int avtrCol;
    int gems;
    Tile** tile;
    tileName avtrIsOn;
} Map;

bool mapInit(Map* map, const char* filename, int* screenW, int* screnH);
void mapUninit(Map* map);
void displayMap(SDL_Window*, SDL_Renderer* ren, Map* map);
int moveAvtr(SDL_Renderer* ren, Map* map, Avtr* avtr, char axis, int spaces);
void drawTile(SDL_Renderer*, tileName, SDL_Rect);
bool isConsumable(tileName type, Avtr* avtr, SDL_Renderer* ren);
int gemsRemaining(Map* map, Avtr* avtr);
bool hasCollision(tileName type);
//int wallEnough(tileName type);


#endif //YESDL_MAP_H
