// Created by casen on 2/15/2024.
// Handels all the textues (loading and unloading)
#include "Texture.h"

/**
 * loads all textures at the start of game
 * @param ren renders thing
 * @param txr The struct holding all of the textures
 */
void initTextures(SDL_Renderer* ren, Assets* txr){
    txr->wall = loadTexture(ren, "..\\assets\\wall.png");
    txr->wallTop = loadTexture(ren, "..\\assets\\wall_top.png");
    txr->key = loadTexture(ren, "..\\assets\\key.png");
    txr->keyVert = loadTexture(ren, "..\\assets\\keyVert.png");
    txr->keySmall = loadTexture(ren, "..\\assets\\key_small.png");
    txr->gem = loadTexture(ren, "..\\assets\\gem.png");
    txr->open_door = loadTexture(ren, "..\\assets\\open_door.png");
    txr->locked_door = loadTexture(ren, "..\\assets\\locked_door.png");
    txr->player = loadTexture(ren, "..\\assets\\player.png");
    txr->button = loadTexture(ren, "..\\assets\\menu.png");
    //txr.type = loadTexture(ren, path);
}

/**
 * turns an image into a texture
 * @param ren renders thing
 * @param file the png being loaded in as a texture
 * @return 
 */
SDL_Texture* loadTexture(SDL_Renderer* ren, const char* file) {
    SDL_Surface* tempSurface = IMG_Load(file);
    if (tempSurface == NULL) {
        tempSurface = IMG_Load("..\\assets\\noTexture.png");
        if (tempSurface == NULL) {
            SDL_Log("tempSurface is NULL\n");
            return 0;
        }
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (texture == NULL){
        SDL_Log("texture is NULL\n");
        return 0;
    }
    return texture;
}

/**
 * Clears all texture call after game is over before application end
 * @param txr The struct holding all of the textures
 */
void destoryTexture(Assets* txr){
    SDL_DestroyTexture(txr->wall);
    SDL_DestroyTexture(txr->wallTop);
    SDL_DestroyTexture(txr->key);
    SDL_DestroyTexture(txr->keyVert);
    SDL_DestroyTexture(txr->keySmall);
    SDL_DestroyTexture(txr->gem);
    SDL_DestroyTexture(txr->open_door);
    SDL_DestroyTexture(txr->locked_door);
    SDL_DestroyTexture(txr->player);
    SDL_DestroyTexture(txr->button);
    //SDL_DestroyTexture(txr.type);
}