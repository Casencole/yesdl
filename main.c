#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "Map.h"

const int SCREEN_W = 640;
const int SCREEN_H = 480;


//const int SIZE = 10;
/**
 * 
 * @param render idk the render thing
 * @param rows the amount of rows this should build a tiny box for
 * @param cols the amount of columns this should build a tiny box for
 * @TODO should make size of squares scalable to the size of the window that I guess would also need to update while running if you changed the size

SDL_Rect drawMap(SDL_Renderer *render, int r, int g, int b){
    //Variables
    int rows = 31;
    int cols = 35;
    int playerRow = 0;
    int playerCol = 0;
    //fscanf(map,"%d %d %d %d", &rows, &cols, &playerRow, &playerCol);
    SDL_SetRenderDrawColor(render, r, g, b, 255);
    SDL_Rect tile[rows][cols];
    for (int i = 0; i < rows; i++) {    //Up-Down -/+
        for (int j = 0; j < cols; j++) {//Left-Right -/+
            tile[i][j].h = 10;
            tile[i][j].w = 10;
            tile[i][j].x = 0 + 10 * j;  //Should move over a column
            tile[i][j].y = 0 + 10 * i;  //Should go down a row
        }
    }
    SDL_RenderFillRect(render, &tile[playerRow][playerCol] );
    SDL_RenderDrawRects(render, *tile,rows * cols);
    return tile[playerRow][playerCol];
}

void clear(SDL_Renderer *render){
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    SDL_RenderPresent(render);
}

void draw(SDL_Renderer *render, SDL_Rect* rect){
    SDL_SetRenderDrawColor(render, 0,59,111,255);
    SDL_RenderFillRect(render, rect);
}

void updatePos(SDL_Renderer* render, SDL_Rect* rect, char xy, int distance){
    clear(render);
    if (xy == 'x'){
        rect->x += distance;
    }else if (xy == 'y'){
        (*rect).y += distance; 
    }
    draw(render, rect);
}
**/

int main(int argc, char** argv) {
    
    //Variables
    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Event e; //Currently used so I can use the close button
    bool quit = false;
    Map map;
    int level = 1;
    
    //Window to render, honesty I barely know how half of this works, but it does
    SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &window, &render);
    
    //File Checking
    if(argc > 1) {
        if (!mapInit(&map, argv[level])) {
            printf("fucking error idk just read files better also why the fuck are you writing code at 1am\n");
            return 0;
        }
        displayMap(&map, render);
    }else {
        printf("did pass any maps i\n");
        return 0;
    }
    
    while (!quit) {
        
        //Checks to see if the event is me hitting the close button & if it is it closes the application
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            } else if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                        moveAvtr(&map, render, 'x', -1);
                        //updatePos(render, &player,'x', -10);
                        break;
                    case SDLK_s:
                        moveAvtr(&map, render, 'y', 1);
                        break;
                    case SDLK_d:
                        moveAvtr(&map, render, 'x', 1);
                        //updatePos(render, &player,'x', 10);
                        break;
                    case SDLK_w:
                        moveAvtr(&map, render, 'y', -1);
                        //updatePos(render, &player,'y', -10);
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
