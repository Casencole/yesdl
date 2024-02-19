#include <stdio.h>

#include <stdbool.h>
#include "SDL2/SDL.h"
#include "Map.h"

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
    Assets assets;
    int level = 1;
    int maxLevel = argc - 1;
    
    
    //Whatever this is
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Error for Initialization of SDL2_image\n");
    }

    //File Checking
    if(argc > 1) {
        if (!mapInit(&map, argv[level], &screenW, &screenH)) {
            SDL_Log("Map did not initialize\n");
            return 0;
        }
        //Window to render, honesty I barely know how half of this works, but it does
        window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, 0);
        render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        initTextures(render, &assets);
        displayMap(render, &map, &assets);
    }else {
        SDL_Log("Did pass in any maps i\n");
        return 0;
    }
    avtrInit(&avtr);
    
    //Main game loop
    while (!quit) {
        int updateDispaly = -1;
        //Button that can be pressed during playtime
        while (SDL_PollEvent(&e)){
            //If you have collected all gems open next level
            if (gemsRemaining(&map, &avtr) == 0 && level <= maxLevel){
                mapUninit(&map);
                level++;
                if (!mapInit(&map, argv[level], &screenW, &screenH)) {
                    SDL_Log("Map did not initialize\n");
                    return 0;
                }
                SDL_SetWindowSize(window, screenW, screenH);
                SDL_SetWindowPosition(window,SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                avtrInit(&avtr);
                displayMap(render, &map, &assets);
                
            //Checks to see if the event is me hitting the close button & if it is it closes the application
            }else if (e.type == SDL_QUIT){
                quit = true;
            } else if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                    case SDLK_LEFT:
                        updateDispaly = moveAvtr(&map, &avtr,'x',-1);
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        updateDispaly = moveAvtr(&map, &avtr,'y',1);
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        updateDispaly = moveAvtr(&map, &avtr, 'x', 1);
                        break;
                    case SDLK_w:
                    case SDLK_UP:
                        updateDispaly = moveAvtr(&map, &avtr, 'y', -1);
                        break;
                    case SDLK_q:
                        quit = true;
                        break; 
                }
            }
            if(updateDispaly == 0) {
                displayInvintory(render, &avtr, assets, screenW);
                displayMap(render, &map, &assets);
            }
        }
        SDL_RenderPresent(render);
    }
    destoryTexture(&assets);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
