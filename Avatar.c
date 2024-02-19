//
// Created by casen on 2/12/2024.
//
#include "Avatar.h"

/**
 * Just sets the values of the invintory to 0 when you start the game
 * @param avtr Struct holding the amount of keys and gems the player has
 */
void avtrInit(Avtr* avtr){
    avtr->keys = 0;
    avtr->gems = 0;
}

void displayInvintory(SDL_Renderer* ren, Avtr* avtr, Assets txr, int screeW){
    //How many yellow tick marks should appear next to the key "symbol" assoicated with how many you have
    SDL_Rect keyTally[4];
    for (int i = 0; i < 4; i++) {
        keyTally[i].x = (screeW - 150) + 20 * (i + 2); //Some sort of math good luck figuring out
        keyTally[i].y = 28;
        keyTally[i].w = 30;
        keyTally[i].h = 30;
    }

    switch (avtr->keys) {
        case 0:
            showKeyAmount(ren, keyTally , 0, txr);
            break;
        case 1:
            showKeyAmount(ren, keyTally , 1, txr);
            break;
        case 2:
            showKeyAmount(ren, keyTally , 2, txr);
            break;
        case 3:
            showKeyAmount(ren, keyTally , 3, txr);
            break;
        case 4:
            showKeyAmount(ren, keyTally , 4, txr);
            break;
        default:
            SDL_SetRenderDrawColor(ren,0,0,0,255);
            SDL_RenderClear(ren);
            for (int i = 0; i < 3; i++) {
                SDL_RenderCopy(ren, txr.keySmall, NULL, &keyTally[i]);
            }
            break;
    }
    
}

static void showKeyAmount(SDL_Renderer* ren, SDL_Rect* keyTally, int amount, Assets txr){
    SDL_SetRenderDrawColor(ren,0,0,0,255);
    SDL_RenderClear(ren);
    for (int i = 0; i < 4; i++) {
        if (i < amount) {
            SDL_RenderCopy(ren, txr.keyVert, NULL, &keyTally[i]);
        }
    }
}