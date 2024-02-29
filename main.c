#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "Map.h"
#include "sll.h"

const int TXTSIZE = 10;// * strlen(displayName)

SDL_Texture* texTotxr(SDL_Renderer*, TTF_Font*, SDL_Color, char*);
void drawMode(SDL_Renderer* ren, Node* menuHead, SDL_Color* colorIndex);
int menuButton(Node* node);

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
    SDL_Event e;
    bool quit = false;
    Node* lvlHead = NULL;
    
    //Whatever this i
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, 0);
    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Assets assets;
    initTextures(ren, &assets);
    
    // ************************************************************** level selection work zone
    //Opens the list of avaible levels and has an int at the top of how many levels are stored in the file
    FILE *inFile = fopen("..\\levels\\levelSelect.txt", "r");
    TTF_Font *font = TTF_OpenFont("..\\assets\\ArialNarrow7.ttf", 500);
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
    
    //Colors
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    int color;// Used in a lot of ternary operation for deciding color
    SDL_Color colorIndex[NUM_COLORS]={{255, 255, 255, 255},  // White TEXT
                                      {0,   0,   0,   255},  // Black BACKGROUND
                                      {0,   59,  111, 255}   // Blue  ACCENT
    };

    
    // Buttons for level selection
    int whIle = 0;
    char* displayName = malloc(sizeof(char*) * 20);
    
    Node* current = listHead;
    while (current != NULL) {
        // Loop through every level and postion the rect in the correct spot
        current->rect->h = 40;
        current->rect->w = screenW/2 - 20;
        current->rect->x = 10 + ( (whIle% 2)  * (screenW / 2));
        current->rect->y = 10 + ( (whIle++/2) * 50);

        // This whole bit just makes sure each name is 10 characters which I think makes it scale better
        // This whole bit just gets rid of the file path and just shows the name
        strcpy(displayName, &current->name[12]);
        int strLength = (int) strlen(displayName);
        displayName[strLength - 4] = '\0';
        strLength = (int) strlen(displayName);
        if (strLength > 10) {
            displayName[10] = '\0';
        } else {
//            SDL_Log("%s", &displayName[strLength - 1]);
//            while(strLength != 11){
//                displayName[strLength++] = ' ';
//            }
//            displayName[11] = '\0';
        }
        
        // position the textRects
        current->textRect->w = strLength * TXTSIZE;
        current->textRect->h = current->rect->h / 2;
        current->textRect->x = (current->rect->x + (current->rect->w / 2)) - (current->textRect->w / 2);
        current->textRect->y = current->rect->y + (current->rect->h / 4);
        
        // Renders the text and the background of each button
        current->txr = texTotxr(ren, font, colorIndex[TEXT], displayName);
        SDL_RenderCopy(ren, assets.button, NULL, current->rect);
        SDL_RenderCopy(ren, current->txr, NULL, current->textRect);
        
        //Give the rect a white outline 
        SDL_SetRenderDrawColor(ren, colorIndex[TEXT].r, colorIndex[TEXT].g, colorIndex[TEXT].b, colorIndex[TEXT].a);
        SDL_RenderDrawRect(ren, current->rect);
        
        current = (Node *) current->next;
    }
    free(displayName);
    
    // Nodes for menu
    // This first one is just the background 
    Node* menuHead = NULL;
    insertNode(&menuHead, "background");
    menuHead->rect->w = screenW;
    menuHead->rect->x = 0;
    menuHead->rect->y = 370;
    menuHead->rect->h = screenH - menuHead->rect->y;
    SDL_SetRenderDrawColor(ren, colorIndex[ACCENT].r, colorIndex[ACCENT].g, colorIndex[ACCENT].b, colorIndex[ACCENT].a);
    SDL_RenderFillRect(ren, menuHead->rect);
    
    // These 3 are the buttons
    insertNode(&menuHead, "make");
    insertNode(&menuHead, "play");
    insertNode(&menuHead, "edit");
    SDL_SetRenderDrawColor(ren, colorIndex[BACKGROUND].r, colorIndex[BACKGROUND].g, colorIndex[BACKGROUND].b, colorIndex[BACKGROUND].a);
    current = (Node *) menuHead->next;
    whIle = 0;
    while (current != NULL){
        current->rect->h = (screenH - menuHead->rect->y) - 30 ;
        current->rect->w = (screenW - 50) / 3;
        current->rect->x = 15 + ((whIle++) * (10 + current->rect->w));
        current->rect->y = menuHead->rect->y + 15;
        SDL_RenderFillRect(ren, current->rect);
        current->txr = texTotxr(ren, font, colorIndex[TEXT], current->name);
        SDL_RenderCopy(ren, current->txr, NULL, current->rect);
        current = (Node *) current->next;
    }
    
    SDL_RenderPresent(ren);
    
    //Main Menu Loop
    while(!quit) {      //quit means continue to game here 
        int updateDispaly = -1;
        while (SDL_PollEvent(&e)) {
            Node** node = NULL;//Node that is clicked on
            int x, y;   //Mouse position
            switch (e.type) {
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym != SDLK_c && e.key.keysym.sym != SDLK_q) break;
                case SDL_QUIT: //Handeling SDL_Quit allows you to use window features like close button and move window around
                    quit = true;
                    break;
                case SDL_MOUSEWHEEL:
                    // Scrool to see more levels
                    if (e.wheel.y > 0) { // scroll up
                        updateDispaly = updatePos(ren, &listHead, 5, colorIndex);
                    } else if (e.wheel.y < 0) {// scrool down
                        updateDispaly = updatePos(ren, &listHead, -5, colorIndex);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&x, &y);
                    SDL_Log("Mouse Position: %d, %d \n", x, y);

                    // If mouse click is in the menuHead area check those buttons instead of the level nodes
                    // First bit is just general area && this bit is making sure specfic area
                    if (y >= menuHead->rect->y && (node = isInNode((Node **) &menuHead->next, x, y)) != NULL){
                        if ((*node)->selected == 0){// Pick
                            // Each button does its own thang
                            switch (menuButton(*node)) {
                                    case 1:// Make
                                        SDL_Log("Make mode is not made yet\n");
                                        break;
                                    case 2: // Play
                                        if (lvlHead == NULL){
                                            SDL_Log("No levels selected\n");
                                            continue;// Continue so we don't show the slection on the play button
                                        }
                                        quit = true;
                                        break;
                                    case 3:// Edit
                                        SDL_Log("Edit mode is not made yet\n");
                                        break;
                                    default:
                                        SDL_Log("You somehow clicked something other then make, play or edit");
                                        break;
                                }
                                
                            // Make sure you only can pick one option at a time
                            current = (Node *) menuHead->next;
                            while (current != NULL){
                                    current->selected = 0;
                                    current = (Node *) current->next;
                                }
                            color = TEXT;
                            (*node)->selected = 1;
                               
                            } else {// Un-Select
                                color = ACCENT;
                                (*node)->selected = 0;
                            }
                            SDL_SetRenderDrawColor(ren, colorIndex[color].r, colorIndex[color].g, colorIndex[color].b, colorIndex[color].a);
                            SDL_RenderDrawRect(ren, (*node)->rect);
                    // If mouse click is above menuHead area then check the level nodes
                    } else {
                        node = isInNode(&listHead, x, y);
                        if (node != NULL){
                            if ((*node)->selected == 0){ //Pick
                                color = ACCENT;
                                insertNode(&lvlHead, (*node)->name);
                                (*node)->selected = 1;
                            } else {                    //un pick
                                color = TEXT;
                                removeNode(&lvlHead, (*node)->name);
                                (*node)->selected = 0;
                            }
                            printList(lvlHead);
                            SDL_SetRenderDrawColor(ren, colorIndex[color].r, colorIndex[color].g, colorIndex[color].b, colorIndex[color].a);
                            SDL_RenderDrawRect(ren, (*node)->rect);
                        }
                    }
                    break; 
                default:
                    break;
            }
            drawMode(ren, menuHead, colorIndex);  // Draw the menu on top of everything (turns out I only need this once, so it does not need to be a function but eh) 
            if (updateDispaly == 0) {                 // Was there a change? do I need to update the display?
                
                SDL_RenderPresent(ren);       // Redner the screen at the end of each 
            }
       
        }
    }
    // Some clean up before the next stage of stuff
    quit = false;
    destroyList(&listHead);
    TTF_CloseFont(font);
    SDL_Log("\nINFO: Final lvl selection\n");
    printList(lvlHead);
    // **************************************************************
    
    if (lvlHead == NULL) {
        SDL_Log("Did not select any maps\n");
        return 0;
    }
    
    //Game varibles
    Map map;
    Avtr avtr;
    current = (Node *) lvlHead;

    do {//This do-while is the game loading each file until there is no more to laod then exits the game 
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
        while (!quit && gemsRemaining(&map, &avtr) != 0) {// This loop is the level (each indivual map) 
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
            if (updateDispaly == 0) {// Was there a change? do I need to update the display?
                displayInvintory(ren, &avtr, assets, screenW);
                displayMap(ren, &map, &assets);
            }
            SDL_RenderPresent(ren);
        }// Unload current map and move to next one if ther is a next one
        SDL_Delay(200);// I thought a slight delay would look better than an "instant" switch to the next map
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

/**
 * Turns a string into a texture
 * @param ren render things
 * @param font the font you want the words to look like
 * @param color the color you want the text to be
 * @param text a string to be made into a texture
 * @return the texture of the the string you inputed
 */
SDL_Texture* texTotxr(SDL_Renderer* ren, TTF_Font* font, SDL_Color color, char* text) {
    SDL_Surface* temp = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
    return texture;
}

/**
 * just draws all the menu stuff I thought I would need to call it a few time but I was wrong so it just sits down here 
 * only being called once but I guess it helps make the code a bit more readable
 * @param ren render things
 * @param menuHead the head node of the menu stuff
 * @param colorIndex the array of all colors 
 */
void drawMode(SDL_Renderer* ren, Node* menuHead, SDL_Color* colorIndex){
    int i = 0, color;
    Node* current = menuHead;
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
        current = (Node *) current->next;
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
int menuButton(Node* node){
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

void drawlevels(SDL_Renderer* ren, Node* head){
    Node* current = head;
    while (current != NULL){
        
        
        current = (Node *) current->next;
    }
}