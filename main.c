#include <stdio.h>
#define NO_STDIO_REDIRECT
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "Map.h"
const int SCREEN_W = 640;
const int SCREEN_H = 480;


int main(int argc, char** argv) {
    
    //Variables
    SDL_Window* window = NULL;
    int screenW;
    int screenH;
    SDL_Renderer *render = NULL;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Event e;
    bool quit = false;
    Map map;
    Avtr avtr;
    int level = 1;
    int maxLevel = argc - 1;
    
    
//    //Load textures
//    if (IMG_Init(IMG_INIT_PNG) == 0) {
//        printf("Error for Initialization of SDL2_image\n");
//    }
    
    //File Checking
    if(argc > 1) {
        if (!mapInit(&map, argv[level], &screenW, &screenH)) {
            printf("Map did not initialize\n");
            return 0;
        }
        //Window to render, honesty I barely know how half of this works, but it does
        window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, 0);
        render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        displayMap(window, render, &map);

    }else {
        printf("Did pass in any maps i\n");
        return 0;
    }
    avtrInit(&avtr);
    
    //Main game loop
    while (!quit) {
        //Checks to see if the event is me hitting the close button & if it is it closes the application
        while (SDL_PollEvent(&e)){
            //If you have collected all gems open next level
            if (gemsRemaining(&map, &avtr) == 0 && level <= maxLevel){
                mapUninit(&map);
                level++;
                if (!mapInit(&map, argv[level], &screenW, &screenH)) {
                    printf("Map did not initialize\n");
                    return 0;
                }
                SDL_SetWindowSize(window, screenW, screenH);
                SDL_SetWindowPosition(window,SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                avtrInit(&avtr);
                displayMap(window, render, &map);
            //Button that can be pressed during playtime
            }else if (e.type == SDL_QUIT){
                quit = true;
            } else if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                    case SDLK_LEFT:
                        moveAvtr(render, &map,&avtr,'x',-1);
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        moveAvtr(render, &map,&avtr,'y',1);
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        moveAvtr(render,&map,&avtr, 'x', 1);
                        break;
                    case SDLK_w:
                    case SDLK_UP:
                        moveAvtr(render, &map,&avtr, 'y', -1);
                        break;
                    case SDLK_q:
                        quit = true;
                        break; 
                }
            }
            displayInvintory(render, &avtr);
        }
        SDL_RenderPresent(render);
    }
    
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    //IMG_Quit();
    SDL_Quit();
    return 0;
}
