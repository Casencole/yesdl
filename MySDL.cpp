//
// Created by casen on 10/29/2024.
//

#include "MySDL.h"

    constexpr SDL_Color MySDL::colors[] = {
        {255, 255, 255, 255}, // White  TEXT
        {0,   0,   0,   255}, // Black  BACKGROUND
        {0,   59,  111, 255}, // Blue   ACCENT
        {200, 5,   5,   255}, // Red    FAIL
        {10,  220, 20,  255}, // Green  SUCCESS
    };

 int MySDL::SDL_Init(){
       if (::SDL_Init(SDL_INIT_EVERYTHING) != 0) {
           SDL_Log("Error for Initialization of SDL\n");
           return -1;
       }
       if (IMG_Init(IMG_INIT_PNG) == 0) {
           SDL_Log("Error for Initialization of SDL2_image\n");
           return -2;
       }
       if (TTF_Init() == -1) {
           SDL_Log("Error for Initialization of TTF\n");
           return -3;
       }
       return 0;
    }
 void MySDL::setDrawColor(int color) const {
        SDL_SetRenderDrawColor(ren, colors[color].r, MySDL::colors[color].g, MySDL::colors[color].b, MySDL::colors[color].a);
    }
 void MySDL::uninit() const {
        TTF_CloseFont(font);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(window);
    }
 void MySDL::quit(){
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
// mySDL

