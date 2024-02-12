//
// Created by casen on 2/10/2024.
//
#include <stdbool.h>
#include "SDL2/SDL.h"
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
    int keys;
    int gems;
}Avtr;

typedef struct {
    bool collison;
    bool consumable;
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
void avtrInit(Avtr* avtr);
//void avtrUninit(Avtr* avtr);
int moveAvtr(SDL_Renderer* renderer, Map* map, Avtr* avtr, char axis, int spaces);
void drawTile(SDL_Renderer* renderer, tileName tile, SDL_Rect );
bool isConsumable(tileName type, Avtr* avtr);
int gemsRemaining(Map* map, Avtr* avtr);
#endif //YESDL_MAP_H
