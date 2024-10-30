//
// Created by casen on 3/1/2024.
//
#include "map.h"
#include "sll.h"

#ifndef YESDL_EDIT_H
#define YESDL_EDIT_H

Button* initTileSelection(int screenW, Assets txr);
void displayTileSelection(MySDL*, Button* head);
void updateTile(int x, int y, Map* map, tileName selected);
int saveFile(Map* map, std::string filename);

#endif //YESDL_EDIT_H
