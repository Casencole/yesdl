/** Q in any mode should quit the game
 *  E in play mode should take you to edit mode
 *  P in edit mode should take you to play mode
 *  WASD or Arrow Keys to move
 */
 
/** List of bugs to fix and things I want to change
 * --After exit on an edit level the level is still in the lvlHead
 * --For the Tile bar I want to make it more scaleable with thte size of the window 
 *      * Have the last 1 or so tiles shown and the save/exit/ A show all tile type menu
 * --Get typing working for saving a level 
 **/

#include <stdio.h>
#include "SDL2/SDL.h"
#include "Map.h"
#include "Edit.h"
#include "sll.h"

const int SIZE = 30;
const int SIDE_SIZE = 120;
const int TXTSIZE = 10;// strlen(displayName) * TXTSIZE size of text rect
const int DFLT_SCREEN_W = 640;
const int DFLT_SCREEN_H = 480;
const int EDIT_SCREEN_H = 660;// How much space I need for the eidt menu

void drawlevels(SDL_Renderer* ren, Button* head, Assets txr, SDL_Color* colorIndex);
void drawMode(SDL_Renderer* ren, Button* menuHead, SDL_Color* colorIndex);
void deselectAllButtons(Button* head);
int  menuButton(Button* node);

typedef enum {MAINMENU = -1, PLAY , EDIT, MAKE, QUIT }mode;

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
    
    // Basic Crap + colors
    int screenW = DFLT_SCREEN_W;
    int screenH = DFLT_SCREEN_H;
    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);;
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    
    SDL_Color colorIndex[NUM_COLORS] = {
            {255,255, 255,255}, // White    TEXT
            {0,  0,   0,  255}, // Black    BACKGROUND
            {0,  59,  111,255}, // Blue     ACCENT
            {200,5,   5,  255}, // Red      FAIL
            {10, 220, 20, 255}, // Green    SUCCESS
    };

    // Opens the list of avaible levels and has an int at the top of how many levels are stored in the file
    TTF_Font *font = TTF_OpenFont("..\\assets\\ArialNarrow7.ttf", 150);
    FILE *inFile = fopen("..\\levels\\levelSelect.txt", "r");
    if (inFile == NULL) {
        SDL_Log("level file is null\n");
        return 0;
    }

    // Reads in the potenial levels and adds them to the selection list 
    Button* selectionHead = NULL;
    char name[100]; //Buffer of 100 character for file path should never be that long
    while (fscanf(inFile, "%s ", name) != EOF) {
        //SDL_Log("Read in file path: '%s' \n", name);
        insertNode(&selectionHead, name);
    }
    fclose(inFile);
    
    // Button for the diffrent levels you can chosse 
    char* displayName = malloc(sizeof(char*) * 20);

    int whIle = 0;
    Button* current = selectionHead;
    while (current != NULL) {
        // Loop through every button and postion the rect in the correct spot
        current->rect->h = 40;
        current->rect->w = (screenW / 2) - 20;
        current->rect->x = 10 + ((whIle% 2) * (screenW / 2));
        current->rect->y = 10 + ((whIle++/2) * 50);

        // This whole bit just gets rid of the file path and just shows the name
        strcpy(displayName, &current->name[12]);    // Start at index 12 since the first 0-11 is just the path 
        int strLength = (int) strlen(displayName);  // Find the lenght of the string
        displayName[strLength - 4] = '\0';          // The last 4 character are the '.txt' and don't need that
        strLength = (int) strlen(displayName);      // Check the size of what remains
        if (strLength > 10) {                       // Only want to show the first 10 character on the button
            displayName[10] = '\0';
        }

        // Position the rects that actaully display the text
        current->textRect->w = strLength * TXTSIZE;
        current->textRect->h = current->rect->h / 2;
        current->textRect->x = (current->rect->x + (current->rect->w / 2)) - (current->textRect->w / 2);
        current->textRect->y = current->rect->y + (current->rect->h / 4);
        current->txr = texTotxr(ren, font, colorIndex[TEXT], displayName);// Give the text txr to each button
        
        current = current->next;
    }
    free(displayName);
    
    // Button for Main Menu 
    // This 1st one is just the background rect I just tossed it in with the 3 buttons
    Button* menuHead = NULL;
    insertNode(&menuHead, "background");
    menuHead->rect->w = screenW;
    menuHead->rect->x = 0;
    menuHead->rect->y = 370;
    menuHead->rect->h = screenH - menuHead->rect->y;
    
    // These 3 are the buttons
    insertNode(&menuHead, "make");
    insertNode(&menuHead, "play");
    insertNode(&menuHead, "edit");

    whIle = 0;
    current = menuHead->next;
    while (current != NULL){
        current->rect->h = (screenH - menuHead->rect->y) - 30 ;
        current->rect->w = (screenW - 50) / 3;
        current->rect->x = 15 + ((whIle++) * (10 + current->rect->w));
        current->rect->y = menuHead->rect->y + 15;
        current->txr = texTotxr(ren, font, colorIndex[TEXT], current->name);
        
        current = current->next;
    }
    

    // Stuff for game
    Map map;
    Avtr avtr;
    SDL_Event e;
    Button* lvlHead = NULL;
    int mode = MAINMENU;
    Assets assets;
    initTextures(ren, &assets);
    
    do {
        switch (mode) {
            case MAINMENU:
                // If there are levels already selected de-select them when coming back here
                deselectAllButtons(selectionHead);
                deselectAllButtons(lvlHead);
                
                while (mode == MAINMENU) {
                    SDL_SetWindowSize(window, DFLT_SCREEN_W, DFLT_SCREEN_H);
                    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                    while (SDL_PollEvent(&e)) {
                        Button* node = NULL;// Button that is clicked on
                        int x, y;// Mouse position
                        switch (e.type) {
                            case SDL_KEYDOWN:
                                if (e.key.keysym.sym != SDLK_q) break;
                            case SDL_QUIT: //Handeling SDL_Quit allows you to use window features like close button and move window around
                                mode = QUIT;
                                break;
                            case SDL_MOUSEWHEEL:
                                // Scrool to see more levels
                                if (e.wheel.y > 0) { // scroll up
                                    updatePos(&selectionHead, 10);
                                } else if (e.wheel.y < 0) {// scrool down
                                    updatePos(&selectionHead, -10);
                                }
                                break;
                            case SDL_MOUSEBUTTONDOWN:
                                SDL_GetMouseState(&x, &y);
                                // If mouse click is in the menuHead area check those buttons instead of the level nodes
                                // First bit is just general area && this bit is making sure specfic area
                                if (y >= menuHead->rect->y &&
                                    (node = isInNode(menuHead->next, x, y)) != NULL) {
                                    if (node->selected == 0) {// Pick
                                        // Each button does its own thang
                                        switch (menuButton(node)) {
                                            case 1:// Make
                                                SDL_Log("Make mode is not made yet\n");
                                                mode = MAKE;
                                                break;
                                            case 2: // Play
                                                if (lvlHead == NULL) {
                                                    SDL_Log("No levels selected\n");
                                                    continue;// Continue so we don't show the slection on the play button
                                                }
                                                mode = PLAY;
                                                break;
                                            case 3:// Edit
                                                if (lvlHead == NULL) {
                                                    SDL_Log("No level selected\n");
                                                    continue;// Continue so we don't show the slection on the play button
                                                }
                                                if (lvlHead->next != NULL) {
                                                    SDL_Log("Only Pick one level to edit\n");
                                                    continue;// Continue so we don't show the slection on the play button
                                                }
                                                mode = EDIT;
                                                break;
                                            default:
                                                SDL_Log("You somehow clicked something other then make, play or edit");
                                                break;
                                        }

                                        // Make sure you only can pick one option at a time
                                        deselectAllButtons(menuHead->next);
                                        node->selected = 1;

                                    } else node->selected = 0; //Un-Select
                                 
                                    // If mouse click is above menuHead area then check the level nodes
                                } else {
                                    if ((node = isInNode(selectionHead, x, y)) != NULL) {
                                        if (node->selected == 0) { //Pick
                                            insertNode(&lvlHead, node->name);
                                            node->selected = 1;
                                        } else {                    //Un-Pick
                                            removeNode(&lvlHead, node->name);
                                            node->selected = 0;
                                        }
                                        printList(lvlHead);
                                    }
                                }
                                break;
                            default:
                                break;
                        }
                        SDL_SetRenderDrawColor(ren, colorIndex[BACKGROUND].r, colorIndex[BACKGROUND].g,
                                               colorIndex[BACKGROUND].b, colorIndex[BACKGROUND].a);
                        SDL_RenderClear(ren);
                        drawlevels(ren, selectionHead, assets, colorIndex);
                        drawMode(ren, menuHead, colorIndex);  // Draw the menu on top of everything (turns out I only need this once, so it does not need to be a function but eh) 
                        SDL_RenderPresent(ren);       // Redner the screen at the end of each
                    }
                }
                break;
            case PLAY:
                SDL_SetRenderDrawColor(ren, colorIndex[BACKGROUND].r, colorIndex[BACKGROUND].g,
                                       colorIndex[BACKGROUND].b, colorIndex[BACKGROUND].a);
                SDL_RenderClear(ren);
                current = lvlHead;
                do {//This do-while is the game loading each file until there is no more to laod then exits the game 
                    if (!mapInit(&map, current->name, &screenW, &screenH)) {
                        SDL_Log("%s did not initialize\n", current->name);
                        return 0;
                    }
                    avtrInit(&avtr);
                    SDL_SetWindowSize(window, screenW, screenH);
                    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                    displayMap(ren, &map, &assets);
                    SDL_Log("%s is now displayed\n", current->name);
                    int updateDispaly = -1;
                    while (mode == PLAY && gemsRemaining(&map, &avtr) != 0) {// This loop is the level (each indivual map) 
                        while (SDL_PollEvent(&e)) {
                            if (e.type == SDL_QUIT) {
                                mode = QUIT;
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
                                        mode = QUIT;
                                        break;
                                    case SDLK_e:
                                        mode = EDIT;
                                    default:
                                        break;
                                }
                            }
                        }
                        if (updateDispaly == 0) {// Was there a change? do I need to update the display?
                            displayInvintory(ren, &avtr, assets, screenW);
                            displayMap(ren, &map, &assets);
                        }
                        SDL_RenderPresent(ren);
                    }// Unload current map and move to next one if ther is a next one
                    SDL_Delay(
                            200);// I thought a slight delay would look better than an "instant" switch to the next map
                    mapUninit(&map);
                    current = current->next;
                } while (mode == PLAY && current != NULL);
                if (mode != QUIT && mode != EDIT) {
                    mode = MAINMENU;
                }
                break;
            case MAKE:
                //Nothing here 
                
                break;
            case EDIT:
                if (!mapInit(&map, lvlHead->name, &screenW, &screenH)) {
                    SDL_Log("%s did not initialize\n", lvlHead->name);
                    return 0;
                }
                screenH = (screenH > EDIT_SCREEN_H) ? screenH : EDIT_SCREEN_H;
                SDL_SetWindowSize(window, screenW, screenH);
                SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

                Button* tileHead = initTileSelection(screenW, assets);
                tileName selectedTile = empty;// Starting tile is the empty one
                SDL_Log("%s is now displayed\n", lvlHead->name);
                while (mode == EDIT) {
                    int x, y;
                    Button* button;
                    while (SDL_PollEvent(&e)) {
                        switch (e.type) {
                            case SDL_KEYDOWN:
                                if (e.key.keysym.sym == SDLK_q) {
                                    mode = QUIT;
                                }
                                if(e.key.keysym.sym == SDLK_p) {
                                    mode = PLAY;
                                }
                                break;
                            case SDL_QUIT:
                                mode = QUIT;
                                break;
                            case SDL_MOUSEBUTTONDOWN:
                                SDL_GetMouseState(&x, &y);
                                SDL_Log("Mouse Position: %d, %d \n", x, y);

                                // Pick the tileType you want to place
                                if (x > screenW - SIDE_SIZE && (button = isInNode(tileHead, x, y)) != NULL) {
                                    if (button->selected == 0) {// Select tile
                                        // Unselects the currently selected tile 
                                        deselectAllButtons(tileHead);
                                        button->selected = 1;
                                        selectedTile = atoi(button->name);
                                        
                                        //If Save or Leave do these instead of normal tile selection
                                        if (selectedTile == save) {
                                            if (saveFile(&map, lvlHead->name) == -1) {
                                                SDL_Log("Writing to file Failed");
                                                button->selected = 2;
                                            } else {
                                                SDL_Log("Writing to file was successful");
                                            }
                                        } else if (selectedTile == leave) {
                                            mode = MAINMENU;
                                        }
                                        
                                    } else {//Un-Select tile
                                        button->selected = 0;
                                        selectedTile = empty;
                                    }
                                    // Pick the tile you want to place it in    
                                } else if (x < screenW - SIDE_SIZE){
                                    updateTile(x, y, &map, selectedTile);
                                }
                                break;
                        }
                        // Update display at the end
                        displayMap(ren, &map, &assets);
                        displayTileSelection(ren, tileHead, colorIndex);
                        SDL_RenderPresent(ren);
                    }
                }
                break;
        }
    } while (mode != QUIT);
    
    //Clean up
    TTF_CloseFont(font);
    destroyList(&menuHead);
    destroyList(&selectionHead);
    destroyList(&lvlHead);
    destoryTexture(&assets);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}


/**
 * 
 * @param ren render things
 * @param head the head node of the list of levels
 * @param txr the struct of all textures
 * @param colorIndex the array of all colors 
 */
void drawlevels(SDL_Renderer* ren, Button* head, Assets txr, SDL_Color* colorIndex){
    int color;
    Button* current = head;
    while (current != NULL){
        // Dispaly the textures
        SDL_RenderCopy(ren, txr.button, NULL, current->rect);
        SDL_RenderCopy(ren, current->txr, NULL, current->textRect);
        
        // Display the outline
        color = (current->selected == 1) ? ACCENT : TEXT;
        SDL_SetRenderDrawColor(ren, colorIndex[color].r, colorIndex[color].g, colorIndex[color].b, colorIndex[color].a);
        SDL_RenderDrawRect(ren, current->rect);
        
        current = current->next;
    }
}

/**
 * just draws all the menu stuff I thought I would need to call it a few time but I was wrong so it just sits down here 
 * only being called once but I guess it helps make the code a bit more readable
 * @param ren render things
 * @param menuHead the head node of the menu stuff
 * @param colorIndex the array of all colors 
 */
void drawMode(SDL_Renderer* ren, Button* menuHead, SDL_Color* colorIndex){
    int i = 0, color;
    Button* current = menuHead;
    while (current != NULL){
        color = (i == 0) ? ACCENT : BACKGROUND;// if head, fill background box to ACCENT otherwise fill with BACKGROUND
        SDL_SetRenderDrawColor(ren, colorIndex[color].r, colorIndex[color].g, colorIndex[color].b, colorIndex[color].a);
        SDL_RenderFillRect(ren, current->rect);

        if (i++ != 0 ) {
            color = (current->selected == 0) ? BACKGROUND : TEXT;
        }
        SDL_RenderCopy(ren, current->txr, NULL, current->rect);
        SDL_SetRenderDrawColor(ren, colorIndex[color].r, colorIndex[color].g, colorIndex[color].b, colorIndex[color].a);
        SDL_RenderDrawRect(ren, current->rect);
        
        current = current->next;
    }
}

/**
 * 
 * @param node any random node just cheking to find to see what the name is
 * @return  1 for make
 *          2 for play 
 *          3 for edit
 *         -1 for anything else
 */
int menuButton(Button* node){
    if (strcmp((const char *) node->name, "make") == 0) {
        SDL_Log("%s \n", node->name);
        return 1;
    }
    if (strcmp((const char *) node->name, "play") == 0) {
        SDL_Log("%s \n", node->name);
        return 2;
    }
    if (strcmp((const char *) node->name, "edit") == 0) {
        SDL_Log("%s \n", node->name);
        return 3;
    }
    return -1;
}

/**
 * 
 * @param head 
 */
void deselectAllButtons(Button* head){
    Button* current = head;
    while (current != NULL) {
        current->selected = 0;
        current = current->next;
    }
}




