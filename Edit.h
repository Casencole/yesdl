//
// Created by casen on 3/1/2024.
//
#include "map.h"
#include "sll.h"

#ifndef YESDL_EDIT_H
#define YESDL_EDIT_H

Buttons* initTileSelection(int screenW, Assets txr);
void displayTileSelection(SDL_Renderer*, Buttons* head, SDL_Color*);
void updateTile(int x, int y, Map* map, tileName selected);
int saveFile(Map* map, const char* filename);

#endif //YESDL_EDIT_H
