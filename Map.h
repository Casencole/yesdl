//
// Created by casen on 2/10/2024.
//
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "Avatar.h"
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
  invalid_tile
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

bool mapInit(Map* map, const char* filename);
void mapUninit(Map* map);
void displayMap(SDL_Renderer* renderer, Map* map);

int moveAvtr(SDL_Renderer* renderer, Map* map, Avtr* avtr, char axis, int spaces);
void drawTile(SDL_Renderer* renderer, tileName tile, SDL_Rect);
bool isConsumable(tileName type, Avtr* avtr, SDL_Renderer* renderer);
int gemsRemaining(Map* map, Avtr* avtr);
bool hasCollision(tileName type);

#endif //YESDL_MAP_H
