//
// Created by casen on 2/15/2024.
//
#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL_image.h"
#include "SDL2/SDL_ttf.h"
#ifndef YESDL_TEXTURE_H
#define YESDL_TEXTURE_H

typedef struct {
    SDL_Texture* wall[2];
    SDL_Texture* key[3];
    SDL_Texture* gem;
    SDL_Texture* empty;
    SDL_Texture* door[2];
    SDL_Texture* player;
    SDL_Texture* button;
    SDL_Texture* side;
    SDL_Texture* save;
    SDL_Texture* exit;
    //SDL_Texture* name;
}Assets;

void initTextures(SDL_Renderer*, Assets*);
SDL_Texture* loadTexture(SDL_Renderer*, const char* file);
void destoryTexture(Assets*);
SDL_Texture* texTotxr(SDL_Renderer*, TTF_Font*, SDL_Color, char*);


#endif //YESDL_TEXTURE_H
