//
// Created by casen on 2/15/2024.
//
#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL_image.h"
#ifndef YESDL_TEXTURE_H
#define YESDL_TEXTURE_H



typedef enum {
    wall_Tex, key_Tex, gem_Tex, 
}texture;

typedef struct {
    //SDL_Texture* tiles[1];
    //SDL_Texture* groupOfTextures[count];
}GameTextures;

void loadTextures(SDL_Renderer* ren, SDL_Texture** textures);
SDL_Texture* loadTexture(SDL_Renderer* ren, const char* file);
void destoryTexture(SDL_Texture** textures);

#endif //YESDL_TEXTURE_H
