//
// Created by casen on 2/10/2024.
//
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "Avatar.h"
#include "Texture.h"

#ifndef YESDL_MAP_H
#define YESDL_MAP_H

extern const int SIZE;


typedef enum {
  // Tiles to be used in game  
  empty,
  wall, 
  fake_wall,
  gem, 
  key, 
  open_door, 
  locked_door,
  //Used for level edit/making
  player,
  save,
  leave,
  invalid_tile,
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
    SDL_Rect side; 
} Map;

bool mapInit(Map* map, const char* filename, int* screenW, int* screnH);
void mapUninit(Map* map);
void displayMap(SDL_Renderer* ren, Map* map, Assets* assets);
int moveAvtr(Map* map, Avtr* avtr, char axis, int spaces);
bool isConsumable(tileName type, Avtr* avtr);
int gemsRemaining(Map* map, Avtr* avtr);
bool hasCollision(tileName type);
int wallEnough(tileName type);



#endif //YESDL_MAP_H
