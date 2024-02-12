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

void displayInvintory(SDL_Renderer* render, Avtr* avtr){
    //Big Yelloy block Indicating keys
    SDL_Rect invintory;
    invintory.x = 490;
    invintory.y = 23;
    invintory.w = 30;
    invintory.h = 30;
    SDL_SetRenderDrawColor(render,215,210,0,255);
    SDL_RenderFillRect(render, &invintory);

    //Black K spelt in the big yellow block to help indicate keys
    SDL_SetRenderDrawColor(render,0,0,0,255);
    SDL_RenderDrawLine(render, 500, 30, 500, 45);
    SDL_RenderDrawLine(render, 500, 37, 507, 30);
    SDL_RenderDrawLine(render, 500, 37, 507, 45);
    
    //How many yellow tick marks should appear next to the key "symbol" assoicated with how many you have
    SDL_Rect keyTally[4];
    for (int i = 0; i < 4; i++) {
        keyTally[i].x = 490 + 20 * (i + 2);
        keyTally[i].y = 28;
        keyTally[i].w = 8;
        keyTally[i].h = 20;
    }

    switch (avtr->keys) {
        case 0:
            showKeyAmount(render, keyTally , 0);
            break;
        case 1:
            showKeyAmount(render, keyTally , 1);
            break;
        case 2:
            showKeyAmount(render, keyTally , 2);
            break;
        case 3:
            showKeyAmount(render, keyTally , 3);
            break;
        case 4:
            showKeyAmount(render, keyTally , 4);
            break;
        default:
            showKeyAmount(render, keyTally , 0);
            for (int i = 0; i < 3; i++) {
                keyTally[i].y = 40;
                keyTally[i].h = 8;
            }
            showKeyAmount(render, keyTally , 3);
            break;
    }
    
}

static void showKeyAmount(SDL_Renderer* render, SDL_Rect* keyTally, int amount){
    for (int i = 0; i < 4; i++) {
        if (i < amount){
            SDL_SetRenderDrawColor(render,215,210,0,255);
        } else {
            SDL_SetRenderDrawColor(render,0,0,0,255);
        }
        SDL_RenderFillRect(render, &keyTally[i]);
    }
}