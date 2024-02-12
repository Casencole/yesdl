//
// Created by casen on 2/12/2024.
//
#include "SDL2/SDL.h"
#ifndef YESDL_AVATAR_H
#define YESDL_AVATAR_H

typedef struct {
    int keys;
    int gems;
}Avtr;

void avtrInit(Avtr* avtr);

void displayInvintory(SDL_Renderer* render, Avtr* avtr);

static void showKeyAmount(SDL_Renderer* render, SDL_Rect* keyTally, int amount);

#endif //YESDL_AVATAR_H
