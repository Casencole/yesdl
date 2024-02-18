//
// Created by casen on 2/15/2024.
//
#include "Texture.h"

//What if I just use this as a loadTileTex()
void loadTextures(SDL_Renderer* ren, SDL_Texture** textures){
    textures[wall_Tex] = loadTexture(ren, "..\\assets\\wall.png");
    //textures[...] = loadTexture(ren, path);
}

SDL_Texture* loadTexture(SDL_Renderer* ren, const char* file) {
    SDL_Surface* tempSurface = IMG_Load("file");
    if (tempSurface == NULL){
        printf("idk\n");
        return 0;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (texture == NULL){
        printf("idk part 2\n");
        return 0;
    }
    return texture;
}

void destoryTexture(SDL_Texture** textures){
    for (int i = 0; i < 1; i++) {
        //SDL_DestroyTexture();
    }
}