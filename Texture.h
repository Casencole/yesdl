//
// Created by casen on 2/15/2024.
//
#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL_image.h"
#ifndef YESDL_TEXTURE_H
#define YESDL_TEXTURE_H

typedef struct {
    SDL_Texture* wall;
    SDL_Texture* wallTop;
    SDL_Texture* key;
    SDL_Texture* keyVert;
    SDL_Texture* keySmall; 
    SDL_Texture* gem;
    SDL_Texture* open_door;
    SDL_Texture* locked_door;
    SDL_Texture* player;
    SDL_Texture* button;
    //SDL_Texture* name;
}Assets;

void initTextures(SDL_Renderer*, Assets*);
SDL_Texture* loadTexture(SDL_Renderer*, const char* file);
void destoryTexture(Assets*);

#endif //YESDL_TEXTURE_H
