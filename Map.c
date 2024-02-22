//
// Created by casen on 2/10/2024.
// Map stuff dealing with map
#include "Map.h"
#include <stdio.h>
#include <stdlib.h>

const int SIZE = 30;
const int bgColor[4]= {0, 0, 0, 255};
const int avtrColor[4]= {0, 59, 111, 255};

/**
 * allocates memory and reads level from text and set's each tile to the resepctive type
 * @param map Struct holding a lot of info about the map (amount of gems, rows, cols, avtrRow, avtrCol,
 *                                                       tile avtrIsOn, and 2d array of all ties)
 * @param filename file name of the level/map being read in 
 * @return  true if file is read correctly 
 *          false if file is not found
 */
bool mapInit(Map* map, const char* filename, int* screenW, int* screenH){
    FILE* level = fopen(filename,"r");
    if (level == NULL) {
        return false;
    }
    
    fscanf(level, "%d %d %d %d ", &map->rows, &map->cols, &map->avtrRow, &map->avtrCol);
    map->gems = 0;
    map->avtrIsOn = empty;
    //set the values of screenW and screenH to the correct things
    *screenW = map->cols * SIZE + 120;
    *screenH = map->rows * SIZE;
    
    map -> tile = malloc(sizeof(Tile *) * map->rows);
    for (int i = 0; i < map->rows; i++) {
        map -> tile[i] = malloc(sizeof(Tile ) * map->cols);
        for (int j = 0; j < map->cols; j++) {
            //Every tile should be the same size and placed in this grid order
            map->tile[i][j].rect.h = SIZE;
            map->tile[i][j].rect.w = SIZE;
            map->tile[i][j].rect.x = 0 + SIZE * j;  //Should move over a column
            map->tile[i][j].rect.y = 0 + SIZE * i;  //Should go down a row
            
            //Determines what each tile type is going to be
            char currentTile;
            fscanf(level,"%c ", &currentTile);
            switch (currentTile) {
                case '#':
                    map->tile[i][j].type = wall;
                    break;
                case '.':
                    map->tile[i][j].type = empty;
                    break;
                case 'S':
                    map->tile[i][j].type = fake_wall;
                    break;
                case '*':
                    map->tile[i][j].type = gem;
                    map->gems++;
                    break;
                case '+':
                    map->tile[i][j].type = key;
                    break;
                case '-':
                    map->tile[i][j].type = open_door;
                    break;
                case '=':
                    map->tile[i][j].type = locked_door;
                    break;
                default:
                    map->tile[i][j].type = invalid_tile;
                    break;
            }
        }
    }
    fclose(level);
    return true;
}

/**
 * Deallocates memory to everything and setws all the tiles to NULL
* @param map Struct holding a lot of info about the map (amount of gems, rows, cols, avtrRow, avtrCol,
 *                                                       tile avtrIsOn, and 2d array of all ties)
 */
void mapUninit(Map* map){
    for (int i = 0; i < map->rows; i++) {
        free(map->tile[i]);
        map->tile[i] = NULL;
    }
    free(map->tile);
    map->tile = NULL;
} 

/**
 * 
 * @param renderer renders thing
 * @param map Struct holding a lot of info about the map (amount of gems, rows, cols, avtrRow, avtrCol,
 *                                                       tile avtrIsOn, and 2d array of all ties)
 */
void displayMap(SDL_Renderer* ren, Map* map, Assets* txr){
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; ++j) {
            switch (map->tile[i][j].type) {
                case wall:
                case fake_wall:
                    if (i == map->rows - 1 || (i != map->rows - 1 && wallEnough(map->tile[i + 1][j].type) == 0) ){
                        SDL_RenderCopy(ren, txr->wall, NULL, &map->tile[i][j].rect);
                    } else {
                        SDL_RenderCopy(ren, txr->wallTop, NULL, &map->tile[i][j].rect);
                    }
                    break;
                case empty:
                    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                    SDL_RenderFillRect(ren, &map->tile[i][j].rect);
                    break;
                case gem:
                    SDL_RenderCopy(ren, txr->gem, NULL, &map->tile[i][j].rect);
                    break;
                case key:
                    SDL_RenderCopy(ren, txr->key, NULL, &map->tile[i][j].rect);
                    break;
                case open_door:
                    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                    SDL_RenderFillRect(ren, &map->tile[i][j].rect);
                    SDL_RenderCopy(ren, txr->open_door, NULL, &map->tile[i][j].rect);
                    break;
                case locked_door:
                    SDL_RenderCopy(ren, txr->locked_door, NULL, &map->tile[i][j].rect);
                    break;
                default://invalid_tile
                    SDL_SetRenderDrawColor(ren, 255, 0, 255, 255);
                    SDL_RenderDrawRect(ren, &map->tile[i][j].rect);
                    break;
            }
        }
    }
    SDL_RenderCopy(ren, txr->player, NULL, &map->tile[map->avtrRow][map->avtrCol].rect);
}

/**
 * Handles all the movement for the avatar making sure it's in bounds, not a tile w/ collision, unlocking doors etc
 * @param map The current level the avatar is moving around on
 * @param avtr Struct holding the amount of keys and gems the player has
 * @param axis Should either be an 'x' or 'y' other inputs are not recognized
 *                  'x' (for moving the avatar in the same row) 
 *                  'y' (for moving the avatar in the same col)
 * @param spaces How many tiles the avatar is moving along said axis
 *                  negative for up or left
 *                  positive for down or right
 * @return 0 everything went correct and avtr moved
 *         1 ran into locked_door w/ no keys
 *        -1 hit the outer layer of the map
 *        -2 hit a a tile that has collison
 *        -3 Invalid axis
 */
int moveAvtr(Map* map, Avtr* avtr, char axis, int spaces){
    int returnVal = 0;
    int newRow = map->avtrRow;
    int newCol = map->avtrCol;
    
    if (axis == 'x') {
        newCol += spaces;
    } else if (axis == 'y') {
        newRow += spaces;
    } else return -3; // Invalid axis
    
    //Make sure in bounds
    if (newRow >= map->rows || newRow < 0 || newCol >= map->cols || newCol < 0){
        return -1;
    }
    
    //Check to see if locked_door and if I have key or if it's a tile with collision
    Tile* tile = &map->tile[newRow][newCol];
    if (tile->type == locked_door && avtr->keys > 0){
        tile->type = open_door;
        avtr->keys--;
    } else if (tile->type == locked_door && avtr->keys < 0){
        return 3;
    } 
    if (hasCollision(tile->type)){
        return -2;
    }
    
    //Step 1. check the tile user is moving to, if consumbale, set tile to empty / if not don't change
    tile->type = (isConsumable(tile->type, avtr)) ? empty : tile->type;

    //Step 2.Store the tile until user moves again
    map->avtrIsOn = tile->type;

    //Step 3.Finnaly move user to the new tile
    map->avtrRow = newRow;
    map->avtrCol = newCol;
    return 0;
}

/**
 * check to see if the tile is something the player can pick up/consume and if
 * so increase those items in the player's invintory
 * @param type diffrent possible tiles
 * @param avtr Struct holding the amount of keys and gems the player has 
 * @return if the tile is consumable then returns true false is not
 */
bool isConsumable(tileName type, Avtr* avtr){
    switch (type) {
        case gem:
            //gem ++
            avtr->gems++;
            return true;
        case key:
            avtr->keys++;
            return true;
        default://Anything that is not consumable
            return false;
            
    }
}

/**
 * Check to see if a tile has collision to see if the palyer can move there
 * @param type diffrent possible tiles
 * @return false is you can walk there true if you can
 */
bool hasCollision(tileName type){
    switch (type) {
        case empty:
            return false;
        case fake_wall:
            return false;
        case gem:
            return false;
        case key:
            return false;
        case open_door:
            return false;
        default:// wall, locked_door
            return true;
    }
}

/**
 * Amount of gems remaining in the map
 * @param map Struct holding a lot of info about the map (amount of gems, rows, cols, avtrRow, avtrCol,
 *                                                       tile avtrIsOn, and 2d array of all ties)
 * @param avtr Struct holding the amount of keys and gems the player has
 * @return 
 */
int gemsRemaining(Map* map, Avtr* avtr){
    
    return map->gems - avtr->gems;
}

int wallEnough(tileName type){
    switch (type) {
        case wall:
        case invalid_tile:
        case fake_wall:
        case locked_door:
        case open_door: return 1;
        default: return 0; //Thing that don't count as a wall
    }
}

