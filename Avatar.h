//
// Created by casen on 2/12/2024.
//
#include "MySDL.h"
#include "SDL_image.h"
#include "Texture.h"
#ifndef YESDL_AVATAR_H
#define YESDL_AVATAR_H

extern const int SIDE_SIZE;

typedef struct {
    int keys;
    int gems;
}Avtr;

void avtrInit(Avtr* avtr);
void displayInventory(SDL_Renderer *ren, Avtr *avtr, Assets txr, int screenW);
static void showKeyAmount(SDL_Renderer*, SDL_Rect* keyTally, int amount, Assets);

#endif //YESDL_AVATAR_H
