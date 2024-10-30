//
// Created by casen on 3/1/2024.
//
#include "Edit.h"
#include <string>

/**
 * Just loads all the starting stuff for the Tile Selection
 * @param screenW ...
 * @return the head of list of buttons
 */
Button* initTileSelection(int screenW, Assets txr){
    // Make a button for each type of tile and invalid_tile will always be the last in the enum of tileName
    std::string keyName;
    Button* tileHead = nullptr;
    for (int i = 0; i < invalid_tile; i++){
        keyName = std::to_string(i);
        insertNode(&tileHead, keyName);
    }
    keyName = std::to_string(open_door);
    removeNode(&tileHead, keyName);

    // Set the position of where each button to the correct location
    int whIle = 0;
    Button* current = tileHead;
    while (current != nullptr){
        current->rect->w = SIZE * 2;
        current->rect->h = SIZE * 2;
        current->rect->x = screenW - SIDE_SIZE + (SIDE_SIZE / 4);
        current->rect->y = 20 + (whIle * (current->rect->h + 10));
        
        switch (stoi(current->name)) {
            case empty:
                current->txr = txr.empty;
                break;
            case wall:
            case fake_wall:
                current->txr = txr.wall[0];
                break;
            case gem:
                current->txr = txr.gem;
                break;
            case key:
                current->txr = txr.key[0];
                break;
            case open_door:
                current->txr = txr.gem;
                break;
            case locked_door:
                current->txr = txr.door[0];
                break;
            case player: 
                current->txr = txr.player;
                break;
            case save:
                current->txr = txr.save;
                break;
            case leave:
                current->txr = txr.exit;
                break;
            default:
                break;
        }
        whIle++;
        current = current->next;
    }
    
    
    
    // Return the head
    printList(tileHead);
    return tileHead;
}

/**
 * Displays all the stuff for Tile Selection
 * @param ren renders things
 * @param head head of teh tile selection list
 * @param txr struct of all textures
 */
void displayTileSelection(MySDL* screen, Button* head){
    int color;
    Button* current = head;
    while (current != nullptr){
        SDL_RenderCopy(screen->ren, current->txr, nullptr, current->rect);
        if (current->name == "8"){// 8 is the save button, this is a crap way to do this, I'll fix it later
            color = (current->selected == 1) ? SUCCESS : FAIL;
        } else {
            color = (current->selected == 0) ? BACKGROUND : TEXT;
        }
        //SDL_SetRenderDrawColor(ren, colorIndex[color].r, colorIndex[color].g, colorIndex[color].b, colorIndex[color].a);
        screen->setDrawColor(color);
        SDL_RenderDrawRect(screen->ren, current->rect);
        current = current->next;
    }
}

/**
 * This updates the tile you click on with the current selected tile
 * @param x mouse x value
 * @param y mouse y value
 * @param map the map you are working on
 * @param selected the tile selected
 */
void updateTile(int x, int y, Map* map, tileName selected){
    int col = x / SIZE;
    int row = y / SIZE;
    SDL_Log("Row: %d\n", row);
    SDL_Log("Col: %d\n", col);
    switch (selected) {
        case player:
            if(map->tile[row][col].type == invalid_tile) {
                map->tile[row][col].type = empty;
            }
            map->avtrCol = col;
            map->avtrRow = row;
            break;
        case locked_door:
            if (map->tile[row][col].type == locked_door){
                map->tile[row][col].type = open_door;
            } else {
                map->tile[row][col].type = selected;
            }
            break;
        default:
            map->tile[row][col].type = selected;
            break;
    }

    SDL_Log("Set Tile to Enum Value: %d\n", selected);
}

/**
 * 
 * @param map the map you are saving
 * @param filename the file name you are saving too
 * @return 0 for success
 *        -1 for fail  
 */
int saveFile(Map* map, std::string filename){
    FILE* lvl = std::fopen(filename.c_str(),"w");
    // When writing if file does not exist I just makes a new one I think
    if (lvl == nullptr) {
        SDL_Log("could not load file\n");
        return -1;
    }
    fprintf(lvl, "%d %d\n", map->rows, map->cols);
    fprintf(lvl, "%d %d\n", map->avtrRow, map->avtrCol);
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            //Determines what each char will be from its filename
            char currentTile;
            switch (map->tile[i][j].type) {
                case wall:
                    currentTile = '#';
                    break;
                case empty:
                    currentTile = '.';
                    break;
                case fake_wall:
                    currentTile = 'S';
                    break;
                case gem:
                    currentTile = '*';
                    break;
                case key:
                    currentTile = '+';
                    break;
                case open_door:
                    currentTile = '-';
                    break;
                case locked_door:
                    currentTile = '=';
                    break;
                default://invalid_tile
                    currentTile = 'E';
                    break;
            }
            fprintf(lvl,"%c ", currentTile);
        }
        fprintf(lvl,"\n");
    }
    fclose(lvl);
    return 0;
}

