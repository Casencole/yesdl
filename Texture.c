// Created by casen on 2/15/2024.
// Handels all the textues (loading and unloading)
#include "Texture.h"

/**
 * loads all textures at the start of game
 * @param ren renders thing
 * @param txr The struct holding all of the textures
 */
void initTextures(SDL_Renderer* ren, Assets* txr){
    txr->wall[0] = loadTexture(ren, "..\\assets\\wall2.png");
    txr->wall[1] = loadTexture(ren, "..\\assets\\wall_top2.png");
    txr->key[0] = loadTexture(ren, "..\\assets\\key.png");
    txr->key[1] = loadTexture(ren, "..\\assets\\keyVert.png");
    txr->key[2] = loadTexture(ren, "..\\assets\\key_small.png");
    txr->gem = loadTexture(ren, "..\\assets\\gem.png");
    txr->door[0] = loadTexture(ren, "..\\assets\\locked_door.png");
    txr->door[1] = loadTexture(ren, "..\\assets\\open_door.png");
    txr->player = loadTexture(ren, "..\\assets\\player2.png");
    txr->button = loadTexture(ren, "..\\assets\\menu.png");
    txr->side = loadTexture(ren, "..\\assets\\side.png");
    txr->empty = loadTexture(ren, "..\\assets\\empty2.png");
    txr->save = loadTexture(ren, "..\\assets\\save.png");
    txr->exit = loadTexture(ren, "..\\assets\\exit.png");
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
    SDL_DestroyTexture(txr->wall[0]);
    SDL_DestroyTexture(txr->wall[1]);
    SDL_DestroyTexture(txr->key[0]);
    SDL_DestroyTexture(txr->key[1]);
    SDL_DestroyTexture(txr->key[2]);
    SDL_DestroyTexture(txr->gem);
    SDL_DestroyTexture(txr->door[0]);
    SDL_DestroyTexture(txr->door[1]);
    SDL_DestroyTexture(txr->player);
    SDL_DestroyTexture(txr->button);
    SDL_DestroyTexture(txr->side);
    SDL_DestroyTexture(txr->empty);
    SDL_DestroyTexture(txr->save);
    SDL_DestroyTexture(txr->exit);
    //SDL_DestroyTexture(txr.type);
}

/**
 * Turns a string into a texture
 * @param ren render things
 * @param font the font you want the words to look like
 * @param color the color you want the text to be
 * @param text a string to be made into a texture
 * @return the texture of the the string you inputed
 */
SDL_Texture* texTotxr(SDL_Renderer* ren, TTF_Font* font, SDL_Color color, char* text) {
    SDL_Surface* temp = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
    return texture;
}