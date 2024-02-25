#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "Map.h"
#include "sll.h"





SDL_Texture* texTotxr(SDL_Renderer*, TTF_Font*, char* filename);

int main(int argc, char** argv) {


    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Error for Initialization of SDL\n");
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Error for Initialization of SDL2_image\n");
    }
    if (TTF_Init() == -1) {
        SDL_Log("Error for Initialization of TTF\n");
    }

    //Variables
    SDL_Window *window = NULL;
    int screenW = 640;
    int screenH = 480;
    SDL_Renderer *render = NULL;
    Map map;
    Avtr avtr;
    Assets assets;
    SDL_Event e;
    bool quit = false;
    Node* lvlHead = NULL;
    
    int level = 1;
    int maxLevel = argc - 1;

    //Whatever this is
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, 0);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    // ************************************************************** level select?
    //Opens the list of avaible levels and has an int at the top of how many levels are stored in the file
    FILE *lvlFile = fopen("..\\levels\\levelSelect.txt", "r");
    TTF_Font *font = TTF_OpenFont("..\\assets\\ArialNarrow7.ttf", 255);
    if (lvlFile == NULL) {
        SDL_Log("level file is null\n");
        return 0;
    }
    
    //Reads in the potenial levels
    Node *listHead = NULL;
    char name[100]; //Buffer of 100 character for file path
    while (fscanf(lvlFile, "%s ", name) != EOF) {
        SDL_Log("%s \n", name);
        insertNode(&listHead, name);
    }
    fclose(lvlFile);
    
    //need to clean this up as the size and postion of button are not finale
    Node* current = listHead;
    int i = 0;
    while (current != NULL) {
        SDL_Log("%s\n", current->name);
        current->rect->h = 40;
        current->rect->w = screenW - 220;
        current->rect->x = 10;
        current->rect->y = 10 + (i++ * 50);
        SDL_RenderCopy(render, texTotxr(render, font, &current->name[12]), NULL, current->rect);
        current = (Node *) current->next;
    }
    
    SDL_RenderPresent(render);
    while(!quit) { //quit here just means continue 
        while (SDL_PollEvent(&e)) {
            Node** node;
            int x, y;
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_c){
                        quit = true;
                    }
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&x, &y);
                    SDL_Log("Mouse Position: %d, %d \n", x, y);
                    node = isInNode(&listHead, x, y);
                    if (node != NULL){
                        if ((*node)->selected == 0){ //Pick
                            SDL_SetRenderDrawColor(render, 0, 100, 70, 255);
                            insertNode(&lvlHead, (*node)->name);
                            (*node)->selected = 1;
                        } else {                    //un pick
                            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
                            removeNode(&lvlHead, (*node)->name);
                            (*node)->selected = 0;
                        }
                        SDL_RenderDrawRect(render, (*node)->rect);
                        SDL_RenderPresent(render);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEMOTION:
                    break;
//                default:
//                    SDL_Log("Nothing here");
//                    break;
            }
        }
    }
    quit = false;
    TTF_CloseFont(font);
    // **************************************************************
    
    //File Checking
    if(argc > 1) {
        if (!mapInit(&map, lvlHead->name, &screenW, &screenH)) {
            SDL_Log("Map did not initialize\n");
            return 0;
        }
        //Window to render, honesty I barely know how half of this works, but it does
        SDL_SetWindowSize(window, screenW, screenH);
        SDL_SetWindowPosition(window,SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
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
                    default:
                        SDL_Log("No Button Mapping for Key");
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
    
    //Clean up
    destroyList(&listHead);
    destoryTexture(&assets);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

SDL_Texture* texTotxr(SDL_Renderer* ren, TTF_Font* font, char* filename) {
    SDL_Color black = {255, 255, 255};
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_Surface* temp = TTF_RenderText_Solid(font, filename, black);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
    return texture;
}



