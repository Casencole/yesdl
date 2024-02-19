//
// Created by casen on 2/12/2024.
//
#include "SDL2/SDL.h"
#include "SDL_image.h"
#include "Texture.h"
#ifndef YESDL_AVATAR_H
#define YESDL_AVATAR_H

typedef struct {
    int keys;
    int gems;
}Avtr;

void avtrInit(Avtr* avtr);
void displayInvintory(SDL_Renderer*, Avtr*, Assets, int screenW);
static void showKeyAmount(SDL_Renderer*, SDL_Rect* keyTally, int amount, Assets);

#endif //YESDL_AVATAR_H
