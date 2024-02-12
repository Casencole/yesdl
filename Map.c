//
// Created by casen on 2/10/2024.
// Map stuff dealing with map

#include "Map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
const int SIZE = 10;
const int bgColor[4]= {0, 0, 0, 255};
const int avtrColor[4]= {0, 59, 111, 255};

bool mapInit(Map* map, const char* filename){
    map->gems = 0;
    FILE* level = fopen(filename,"r");
    if (level == NULL) {
        return false;
    }
    fscanf(level, "%d %d %d %d ", &map->rows, &map->cols, &map->avtrRow, &map->avtrCol);
    map -> tile = malloc(sizeof(Tile *) * map->rows);
    for (int i = 0; i < map->rows; i++) {
        map -> tile[i] = malloc(sizeof(Tile ) * map->cols);
        for (int j = 0; j < map->cols; j++) {
            //Every tile should be the same size and placed in this grid order
            map->tile[i][j].rect.h = SIZE;
            map->tile[i][j].rect.w = SIZE;
            map->tile[i][j].rect.x = 10 + SIZE * j;  //Should move over a column
            map->tile[i][j].rect.y = 10 + SIZE * i;  //Should go down a row
            
            //Determines what each tile type is going to be
            char currentTile;
            fscanf(level,"%c ", &currentTile);
            switch (currentTile) {
                case '#':
                    map->tile[i][j].type = wall;
                    map->tile[i][j].collison = true;
                    break;
                case '.':
                    map->tile[i][j].type = empty;
                    map->tile[i][j].collison = false;
                    break;
                case 'S':
                    map->tile[i][j].type = fake_wall;
                    map->tile[i][j].collison = false;
                    break;
                case '*':
                    map->tile[i][j].type = gem;
                    map->tile[i][j].collison = false;
                    map->gems++;
                    break;
                case '+':
                    map->tile[i][j].type = key;
                    map->tile[i][j].collison = false;
                    break;
                case '-':
                    map->tile[i][j].type = open_door;
                    map->tile[i][j].collison = false;
                    break;
                case '=':
                    map->tile[i][j].type = locked_door;
                    map->tile[i][j].collison = true;
                    break;
                default:
                    map->tile[i][j].type = invalid_tile;
                    map->tile[i][j].collison = true;
                    break;
            }
        }
    }
    fclose(level);
    return true;
}

void mapUninit(Map* map){
    for (int i = 0; i < map->rows; i++) {
        free(map->tile[i]);
        map->tile[i] = NULL;
    }
    free(map->tile);
    map->tile = NULL;
}

void displayMap(SDL_Renderer* renderer, Map* map){
    //SDL_SetRenderDrawColor(renderer, 69, 69, 69, 69);
    //SDL_RenderClear(renderer);
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; ++j) {
            drawTile(renderer, map->tile[i][j].type, map->tile[i][j].rect);
            map->avtrIsOn = empty;
            SDL_SetRenderDrawColor(renderer, avtrColor[0], avtrColor[1], avtrColor[2], avtrColor[3]);
            SDL_RenderFillRect(renderer, &map->tile[map->avtrRow][map->avtrCol].rect);
        }
    }
    
}

void avtrInit(Avtr* avtr){
    avtr->keys = 0;
    avtr->gems = 0;
}

//void avtrUninit(Avtr* avtr){
//
//}

/**
 * 
 * @param map The current level the avatar is moving around on
 * @param axis Should either be an 'x' or 'y' other inputs are not recognized
 *                  'x' (for moving the avatar in the same row) 
 *                  'y' (for moving the avatar in the same col)
 * @param spaces How many tiles the avatar is moving along said axis
 *                  negative for up or left
 *                  positive for down or right
 * @return 0  everything went correct
 *         -1 hit the outer layer of the map
 *         -2 hit a a tile that has collison
 *         -3 hit a locked_door and have no keys
 */
int moveAvtr(SDL_Renderer* renderer, Map* map, Avtr* avtr, char axis, int spaces){
    
    //Check to make sure move is in bounds and is not a tile with collsion
    //Also checks to see if it is a door that can be unlocked
    if (axis == 'x'){
        if (map->avtrCol + spaces >= map->cols || map->avtrCol + spaces < 0){
            return -1; //error of out of bound can't move
        } else if (map->tile[map->avtrRow][map->avtrCol + spaces].type == locked_door){
            if (avtr->keys > 0) {
                map->tile[map->avtrRow][map->avtrCol + spaces].type = open_door;
                map->tile[map->avtrRow][map->avtrCol + spaces].collison = false;
                avtr->keys--;
            } else return -3;
        } else if (map->tile[map->avtrRow][map->avtrCol + spaces].collison){
            return -2;
        }
    } else if (axis == 'y'){
        if (map->avtrRow + spaces >= map->rows || map->avtrRow + spaces < 0){
            return -1; //error of out of bounds can't move
        } else if (map->tile[map->avtrRow + spaces][map->avtrCol].type == locked_door){
            if (avtr->keys > 0) {
                map->tile[map->avtrRow + spaces][map->avtrCol].type = open_door;
                map->tile[map->avtrRow + spaces][map->avtrCol].collison = false;
                avtr->keys--;
            } else return -3;
        } else if (map->tile[map->avtrRow + spaces][map->avtrCol].collison){
            return -2;
        }
    }
    
    //Redrawing the tile that the player is one or if consumed drawing an empty tile
    if (isConsumable(map->avtrIsOn, avtr)){
        drawTile(renderer, empty, map->tile[map->avtrRow][map->avtrCol].rect);
    }else {
        drawTile(renderer, map->avtrIsOn, map->tile[map->avtrRow][map->avtrCol].rect);
    }

    //Setting the new position to the avatar tile
    if (axis == 'x'){
        map->avtrIsOn = map->tile[map->avtrRow][map->avtrCol + spaces].type;
        map->avtrCol += spaces;
    } else if (axis =='y'){
        map->avtrIsOn = map->tile[map->avtrRow + spaces][map->avtrCol].type;
        map->avtrRow += spaces;
        
    }
    
    //Drawing the new position of the avatar
    SDL_SetRenderDrawColor(renderer, avtrColor[0], avtrColor[1], avtrColor[2], avtrColor[3]);
    SDL_RenderFillRect(renderer, &map->tile[map->avtrRow][map->avtrCol].rect);
    return 0;
}

void drawTile(SDL_Renderer* renderer, tileName type, SDL_Rect rect){
    switch (type) {
        case wall:
            SDL_SetRenderDrawColor(renderer,61,71,62,255);
            SDL_RenderDrawRect(renderer, &rect);
            break;
        case empty:
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderFillRect(renderer, &rect);
            break;
        case fake_wall:
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer,61,71,62,255);
            SDL_RenderDrawRect(renderer, &rect);
            break;
        case gem:
            SDL_SetRenderDrawColor(renderer,0,215,69,255);
            SDL_RenderFillRect(renderer, &rect);
            break;
        case key:
            SDL_SetRenderDrawColor(renderer,215,210,0,255);
            SDL_RenderFillRect(renderer, &rect);
            break;
        case open_door:
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer,147,124,70,255);
            SDL_RenderDrawRect(renderer, &rect);
            break;
        case locked_door:
            SDL_SetRenderDrawColor(renderer,147,124,70,255);
            SDL_RenderFillRect(renderer, &rect);
            break;
        default://invalid_tile
            SDL_SetRenderDrawColor(renderer,255,0,255,255);
            SDL_RenderDrawRect(renderer, &rect);
            break;
    }
}

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

int gemsRemaining(Map* map, Avtr* avtr){
    return map->gems - avtr->gems;
}