/** Q in any mode should quit the game
 *  E in play mode should take you to edit mode
 *  P in edit mode should take you to play mode
 *  WASD or Arrow Keys to move
 */
 
/** List of bugs to fix and things I want to change
 * --After exit on an edit level the level is still in the lvlHead
 * --For the Tile bar I want to make it more scalable with the size of the window
 *      * Have the last 1 or so tiles shown and the save/exit/ A show all tile type menu
 * --Get typing working for saving a level 
 **/

#include <SDL.h>
#include "Map.h"
#include "Edit.h"
#include "sll.h"
#include "Texture.h"
#include "MySDL.h"

const int SIDE_SIZE     = 120;
const int TXTSIZE       = 10;// strlen(displayName) * TXTSIZE size of text rect
const int DFLT_SCREEN_W = 640;
const int DFLT_SCREEN_H = 480;
const int EDIT_SCREEN_H = 660;// How much space I need for the edit menu
const int MAX_FILE_PATH_LENGTH = 100; //Buffer of 100 character for file path should never be that long
const char* levelFile   = "../scripts/levels/levelSelect.txt";
const char* fontFile    = "../scripts/assets/ArialNarrow7.ttf";

void drawLevels(MySDL* screen, Button* head, Assets txr);
void drawMode(MySDL* screen, Button* menuHead);
int  menuButton(Button* node);

typedef enum {MAINMENU = -1, PLAY , EDIT, MAKE, QUIT }mode;

int main(int argc, char** argv) {
    if (MySDL::SDL_Init() != 0) return -1;
    auto* screen = new MySDL;
    // Basic Crap + colors
    int screenW = DFLT_SCREEN_W;
    int screenH = DFLT_SCREEN_H;
    screen->window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, 0);
    screen->ren = SDL_CreateRenderer(screen->window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(screen->ren, SDL_BLENDMODE_BLEND);

    // Opens the list of available levels and has an int at the top of how many levels are stored in the file
    screen->font = TTF_OpenFont(fontFile, 150);

    FILE *inFile = fopen(levelFile, "r");
    if (inFile == nullptr) {
        SDL_Log("level file not found \n");
        return 0;
    }

    // Reads in the potential levels and adds them to the selection list
    Button* selectionHead = nullptr;
    char name[MAX_FILE_PATH_LENGTH];
    while (fscanf(inFile, "%s ", name) != EOF) {
        insertNode(&selectionHead, name);
    }
    fclose(inFile);
    
    // Button for the different levels you can choose
    char* displayName = (char*)malloc(sizeof(char*) * 20);

    int whIle = 0;
    Button* current = selectionHead;
    while (current != nullptr) {
        // Loop through every button and position the rect in the correct spot
        current->rect->h = 40;
        current->rect->w = (screenW / 2) - 20;
        current->rect->x = 10 + ((whIle% 2) * (screenW / 2));
        current->rect->y = 10 + ((whIle++/2) * 50);

        // This whole bit just gets rid of the file path and just shows the name
        strcpy(displayName, &current->name[18]);    // Start at index 12 since the first 0-11 is just the path
        int strLength = (int) strlen(displayName);  // Find the length of the string
        displayName[strLength - 4] = '\0';          // The last 4 character are the '.txt' and don't need that
        strLength = (int) strlen(displayName);      // Check the size of what remains
        if (strLength > 10) {                       // Only want to show the first 10 character on the button
            displayName[10] = '\0';
        }

        // Position the rects that actually display the text
        current->textRect->w = strLength * TXTSIZE;
        current->textRect->h = current->rect->h / 2;
        current->textRect->x = (current->rect->x + (current->rect->w / 2)) - (current->textRect->w / 2);
        current->textRect->y = current->rect->y + (current->rect->h / 4);
        current->txr = texTotxr(screen, displayName);// Give the text txr to each button
        
        current = current->next;
    }
    free(displayName);
    
    // Button for Main Menu 
    // This 1st one is just the background rect I just tossed it in with the 3 buttons
    Button* menuHead = nullptr;
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
    while (current != nullptr){
        current->rect->h = (screenH - menuHead->rect->y) - 30 ;
        current->rect->w = (screenW - 50) / 3;
        current->rect->x = 15 + ((whIle++) * (10 + current->rect->w));
        current->rect->y = menuHead->rect->y + 15;
        current->txr = texTotxr(screen, current->name);
        
        current = current->next;
    }
    

    // Stuff for game
    Map map;
    Avtr avtr;
    SDL_Event e;
    Button* lvlHead = nullptr;
    int mode = MAINMENU;
    Assets assets;
    initTextures(screen->ren, &assets);
    
    do {
        switch (mode) {
            default:
            case MAINMENU:
                // If there are levels already selected de-select them when coming back here
                deselectAllButtons(menuHead);
                deselectAllButtons(selectionHead);
                deselectAllButtons(lvlHead);
                clearList(&lvlHead);
                SDL_SetWindowSize(screen->window, DFLT_SCREEN_W, DFLT_SCREEN_H);
                SDL_SetWindowPosition(screen->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

                while (mode == MAINMENU) {
                    while (SDL_PollEvent(&e)) {
                        Button* node = nullptr;// Button that is clicked on
                        int x, y;// Mouse position
                        switch (e.type) {
                            case SDL_KEYDOWN:
                                if (e.key.keysym.sym != SDLK_q) break;
                            case SDL_QUIT: //Handling SDL_Quit allows you to use window features like close button and move window around
                                mode = QUIT;
                                break;
                            case SDL_MOUSEWHEEL:
                                // Scroll to see more levels
                                if (e.wheel.y > 0) { // scroll up
                                    updatePos(&selectionHead, 10);
                                } else if (e.wheel.y < 0) {// scroll down
                                    updatePos(&selectionHead, -10);
                                }
                                break;
                            case SDL_MOUSEBUTTONDOWN:
                                SDL_GetMouseState(&x, &y);
                                // If mouse click is in the menuHead area check those buttons instead of the level nodes
                                // First bit is just general area && this bit is making sure specific area
                                if (y >= menuHead->rect->y &&
                                    (node = isInNode(menuHead->next, x, y)) != nullptr) {
                                    if (node->selected == 0) {// Pick
                                        // Each button does its own thang
                                        switch (menuButton(node)) {
                                            case 1:// Make
                                                mode = MAKE;
                                                break;
                                            case 2: // Play
                                                if (lvlHead == nullptr) {
                                                    SDL_Log("No levels selected\n");
                                                    continue;// Continue so we don't show the selection on the play button
                                                }
                                                mode = PLAY;
                                                break;
                                            case 3:// Edit
                                                if (lvlHead == nullptr) {
                                                    SDL_Log("No level selected\n");
                                                    continue;// Continue so we don't show the selection on the play button
                                                }
                                                if (lvlHead->next != nullptr) {
                                                    SDL_Log("Only Pick one level to edit\n");
                                                    continue;// Continue so we don't show the selection on the play button
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
                                    if ((node = isInNode(selectionHead, x, y)) != nullptr) {
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
                        screen->setDrawColor(MySDL::BACKGROUND);
                        SDL_RenderClear(screen->ren);
                        drawLevels(screen, selectionHead, assets);
                        drawMode(screen, menuHead);  // Draw the menu on top of everything (turns out I only need this once, so it does not need to be a function but eh)
                        SDL_RenderPresent(screen->ren);       // Render the screen at the end of each
                    }
                }
                break;
            case PLAY:
                screen->setDrawColor(MySDL::BACKGROUND);
                SDL_RenderClear(screen->ren);
                current = lvlHead;
                do {//This do-while is the game loading each file until there is no more to load then exits the game
                    if (!mapInit(&map, current->name, &screenW, &screenH)) {
                        SDL_Log("%s did not initialize\n", current->name.c_str());
                        return 0;
                    }
                    avtrInit(&avtr);
                    SDL_SetWindowSize(screen->window, screenW, screenH);
                    SDL_SetWindowPosition(screen->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                    displayMap(screen->ren, &map, &assets);
                    SDL_Log("%s is now displayed\n", current->name.c_str());
                    int updateDisplay = -1;
                    while (mode == PLAY && gemsRemaining(&map, &avtr) != 0) {// This loop is the level (each individual map)
                        while (SDL_PollEvent(&e)) {
                            if (e.type == SDL_QUIT) {
                                mode = QUIT;
                            } else if (e.type == SDL_KEYDOWN) {
                                switch (e.key.keysym.sym) {
                                    case SDLK_a:
                                    case SDLK_LEFT:
                                        updateDisplay = moveAvtr(&map, &avtr, 'x', -1);
                                        break;
                                    case SDLK_s:
                                    case SDLK_DOWN:
                                        updateDisplay = moveAvtr(&map, &avtr, 'y', 1);
                                        break;
                                    case SDLK_d:
                                    case SDLK_RIGHT:
                                        updateDisplay = moveAvtr(&map, &avtr, 'x', 1);
                                        break;
                                    case SDLK_w:
                                    case SDLK_UP:
                                        updateDisplay = moveAvtr(&map, &avtr, 'y', -1);
                                        break;
                                    case SDLK_q:
                                        mode = QUIT;
                                        break;
                                    case SDLK_e:
                                        mode = EDIT;
                                        break;
                                    case SDLK_m:
                                        mode = MAINMENU;
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                        if (updateDisplay == 0) {// Was there a change? do I need to update the display?
                            displayInventory(screen->ren, &avtr, assets, screenW);
                            displayMap(screen->ren, &map, &assets);
                        }
                        SDL_RenderPresent(screen->ren);
                    }// Unload current map and move to next one if there is a next one
                    SDL_Delay(
                            200);// I thought a slight delay would look better than an "instant" switch to the next map
                    mapUninit(&map);
                    if (mode != EDIT) removeNode(&lvlHead, current->name);
                    current = current->next;
                } while (mode == PLAY && current != nullptr);
                if (mode != QUIT && mode != EDIT) {
                    mode = MAINMENU;
                }
                break;
            case MAKE: {
                deselectAllButtons(lvlHead);
                clearList(&lvlHead);
                // Write new level
                FILE *lvlList = fopen(levelFile, "a");
                fprintf(lvlList, "../scripts/levels/1.txt\n");
                fclose(lvlList);

                // Write template data for new level
                std::string newLevel = "../scripts/levels/1.txt";
                FILE *lvl = std::fopen(newLevel.c_str(), "w");
                fprintf(lvl, "%d %d\n", 12, 12);
                fprintf(lvl, "%d %d\n", 1, 1);
                for (int i = 0; i < 12; i++) {
                    for (int j = 0; j < 12; j++) {
                        fprintf(lvl, ". ");
                    }
                    fprintf(lvl, "\n");
                }
                fclose(lvl);
                insertNode(&lvlHead, newLevel);
                mode = EDIT;
            }break;
            case EDIT:
                if (!mapInit(&map, lvlHead->name, &screenW, &screenH)) {
                    SDL_Log("%s did not initialize\n", lvlHead->name.c_str());
                    return 0;
                }
                screenH = (screenH > EDIT_SCREEN_H) ? screenH : EDIT_SCREEN_H;
                SDL_SetWindowSize(screen->window, screenW, screenH);
                SDL_SetWindowPosition(screen->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

                Button* tileHead = initTileSelection(screenW, assets);
                tileName selectedTile = empty;// Starting tile is the empty one
                SDL_Log("%s is now displayed in EDIT mode\n", lvlHead->name.c_str());
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
                                if (x > screenW - SIDE_SIZE && (button = isInNode(tileHead, x, y)) != nullptr) {
                                    if (button->selected == 0) {// Select tile
                                        // Unselects the currently selected tile 
                                        deselectAllButtons(tileHead);
                                        button->selected = 1;
                                        selectedTile = (tileName)stoi(button->name);
                                        
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
                        displayMap(screen->ren, &map, &assets);
                        displayTileSelection(screen, tileHead);
                        SDL_RenderPresent(screen->ren);
                    }
                }
                break;
        }
    } while (mode != QUIT);
    
    //Clean up
    destroyList(&menuHead);
    destroyList(&selectionHead);
    destroyList(&lvlHead);
    destroyTexture(&assets);
    screen->uninit();
    MySDL::quit();
    return 0;
}


/**
 * @param ren render things
 * @param head the head node of the list of levels
 * @param txr the struct of all textures
 * @param colorIndex the array of all colors
 */
void drawLevels(MySDL* screen, Button* head, Assets txr){
    int color;
    Button* current = head;
    while (current != nullptr){
        // Display the textures
        SDL_RenderCopy(screen->ren, txr.button, nullptr, current->rect);
        SDL_RenderCopy(screen->ren, current->txr, nullptr, current->textRect);

        // Display the outline
        color = (current->selected == 1) ? ACCENT : TEXT;
        screen->setDrawColor(color);
        SDL_RenderDrawRect(screen->ren, current->rect);
        
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
void drawMode(MySDL* screen, Button* menuHead){
    int i = 0, color;
    Button* current = menuHead;
    while (current != nullptr){
        color = (i == 0) ? ACCENT : BACKGROUND;// if head, fill background box to ACCENT otherwise fill with BACKGROUND
        screen->setDrawColor(color);
        SDL_RenderFillRect(screen->ren, current->rect);

        if (i++ != 0 ) {
            color = (current->selected == 0) ? BACKGROUND : TEXT;
        }
        SDL_RenderCopy(screen->ren, current->txr, nullptr, current->rect);
        screen->setDrawColor(color);
        SDL_RenderDrawRect(screen->ren, current->rect);
        
        current = current->next;
    }
}

/**
 * @param node any random node just checking to find to see what the name is
 * @return  1 for make
 *          2 for play 
 *          3 for edit
 *         -1 for anything else
 */
int menuButton(Button* node){
    if (node->name == "make") {
        SDL_Log("%s \n", node->name.c_str());
        return 1;
    }
    if (node->name == "play") {
        SDL_Log("%s \n", node->name.c_str());
        return 2;
    }
    if (node->name == "edit") {
        SDL_Log("%s \n", node->name.c_str());
        return 3;
    }
    return -1;
}
