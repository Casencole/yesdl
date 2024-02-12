#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "Map.h"

const int SCREEN_W = 640;
const int SCREEN_H = 480;

int main(int argc, char** argv) {
    
    //Variables
    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Event e; //Currently used so I can use the close button
    bool quit = false;
    Map map;
    Avtr avtr;
    int level = 1;
    
    //Window to render, honesty I barely know how half of this works, but it does
    SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &window, &render);
    
    //File Checking
    if(argc > 1) {
        if (!mapInit(&map, argv[level])) {
            printf("map did not initialize\n");
            return 0;
        }
        displayMap(render, &map);
    }else {
        printf("did pass in any maps i\n");
        return 0;
    }
    avtrInit(&avtr);
    
    //Main game loop
    while (!quit) {
        //Checks to see if the event is me hitting the close button & if it is it closes the application
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT || gemsRemaining(&map, &avtr) == 0){
                quit = true;
            } else if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                        moveAvtr(render, &map,&avtr,'x',-1);
                        break;
                    case SDLK_s:
                        moveAvtr(render, &map,&avtr,'y',1);
                        break;
                    case SDLK_d:
                        moveAvtr(render,&map,&avtr, 'x', 1);
                        break;
                    case SDLK_w:
                        moveAvtr(render, &map,&avtr, 'y', -1);
                        break;
                    case SDLK_q:
                        quit = true;
                        break; 
                }
            } 
        }
        SDL_RenderPresent(render);
    }
    mapUninit(&map);
    SDL_Quit();
    return 0;
}
