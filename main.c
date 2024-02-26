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
    SDL_Renderer *ren = NULL;
    Map map;
    Avtr avtr;
    Assets assets;
    SDL_Event e;
    bool quit = false;
    Node* lvlHead = NULL;



    
    
    //Whatever this is
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, 0);
    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    // ************************************************************** level selection work zone
    //Opens the list of avaible levels and has an int at the top of how many levels are stored in the file
    FILE *inFile = fopen("..\\levels\\levelSelect.txt", "r");
    TTF_Font *font = TTF_OpenFont("..\\assets\\ArialNarrow7.ttf", 255);
    if (inFile == NULL) {
        SDL_Log("level file is null\n");
        return 0;
    }
    
    //Reads in the potenial levels
    Node *listHead = NULL;
    char name[100]; //Buffer of 100 character for file path
    while (fscanf(inFile, "%s ", name) != EOF) {
        SDL_Log("Read in file path: '%s' \n", name);
        insertNode(&listHead, name);
    }
    fclose(inFile);
    
    //need to clean this up as the size and postion of button are not finale
    Node* current = listHead;
    int i = 0;
    while (current != NULL) {
        SDL_Log("Displayed: '%s'\n", current->name);
        current->rect->h = 40;
        current->rect->w = screenW - 220;
        current->rect->x = 10;
        current->rect->y = 10 + (i++ * 50);
        SDL_RenderCopy(ren, texTotxr(ren, font, &current->name[12]), NULL, current->rect);
        current = (Node *) current->next;
    }
    
    SDL_RenderPresent(ren);
    while(!quit) { //quit here just means continue 
        while (SDL_PollEvent(&e)) {
            Node** node;//Node that is clicked on
            int x, y;   //Mouse position
            switch (e.type) {
                //Checks to see if the event is me hitting the close button & if it is it closes the application (allows me to move the window as well)
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_c || e.key.keysym.sym == SDLK_q){
                        quit = true;
                    }
                    break;//This break caused me sooo much pain b/c I forgot to put it in somehow and way so confused why my linked list was broken or my loops
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&x, &y);
                    SDL_Log("Mouse Position: %d, %d \n", x, y);
                    node = isInNode(&listHead, x, y);
                    if (node != NULL){
                        if ((*node)->selected == 0){ //Pick
                            SDL_SetRenderDrawColor(ren, 0, 100, 70, 255);
                            insertNode(&lvlHead, (*node)->name);
                            printList(lvlHead);
                            (*node)->selected = 1;
                        } else {                    //un pick
                            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                            removeNode(&lvlHead, (*node)->name);
                            printList(lvlHead);
                            (*node)->selected = 0;
                        }
                        SDL_RenderDrawRect(ren, (*node)->rect);
                        SDL_RenderPresent(ren);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    quit = false;
    destroyList(&listHead);
    TTF_CloseFont(font);
    SDL_Log("\nINFO: Final lvl selection\n");
    // **************************************************************
    
    printList(lvlHead);
    if (lvlHead == NULL) {
        SDL_Log("Did not select any maps\n");
        return 0;
    }

    initTextures(ren, &assets);
    current = (Node *) lvlHead;

    do {
        if (!mapInit(&map, current->name, &screenW, &screenH)) {
            SDL_Log("%s did not initialize\n", lvlHead->name);
            return 0;
        }
        avtrInit(&avtr);
        SDL_SetWindowSize(window, screenW, screenH);
        SDL_SetWindowPosition(window,SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        displayMap(ren, &map, &assets);
        SDL_Log("%s is now displayed\n", lvlHead->name);
        int updateDispaly = -1;
        while (!quit && gemsRemaining(&map, &avtr) != 0) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_a:
                        case SDLK_LEFT:
                            updateDispaly = moveAvtr(&map, &avtr, 'x', -1);
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:
                            updateDispaly = moveAvtr(&map, &avtr, 'y', 1);
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
                            break;
                    }
                }
            }
            if (updateDispaly == 0) {
                displayInvintory(ren, &avtr, assets, screenW);
                displayMap(ren, &map, &assets);
            }
            SDL_RenderPresent(ren);
        }
        SDL_Delay(200);//Not needede but I thought it may look better with a slight delay, so it's not instant when you collect all gems
        mapUninit(&map);
        current = (Node *) current->next;
    } while (!quit && current != NULL);

    //Clean up
    destroyList(&lvlHead);
    destoryTexture(&assets);
    SDL_DestroyRenderer(ren);
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