//
// Created by casen on 2/15/2024.
//
#include "string"
#include "MySDL.h"

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
void destroyTexture(Assets* txr);
SDL_Texture* texTotxr(MySDL* screen, const std::string&);


#endif //YESDL_TEXTURE_H
