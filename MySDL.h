//
// Created by casen on 10/29/2024.
//

#ifndef NEWYESDL_SDL_H
#define NEWYESDL_SDL_H
#include <SDL.h>
#include "SDL_ttf.h"
#include "SDL_image.h"
    class MySDL {
    public:
        int screenW, screenH;
        SDL_Window *window;
        SDL_Renderer *ren;
        TTF_Font *font;

        typedef enum {TEXT, BACKGROUND, ACCENT, FAIL, SUCCESS}RGB;
        static const SDL_Color colors[];


        static int SDL_Init();
        void uninit() const;
        static void quit();
        void setDrawColor(int color) const;
    private:

    };

#endif //NEWYESDL_SDL_H
