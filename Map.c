//
// Created by casen on 2/10/2024.
//  Map stuff dealing with map

#include "Map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
const int SIZE = 10;

bool mapInit(Map* map, const char* filename){
    FILE* level = fopen(filename,"r");
    if (level == NULL) {
        return false;
    }
    fscanf(level, "%d %d %d %d ", &map->rows, &map->cols, &map->avtrRow, &map->avtrCols);
    fclose(level);
    map -> tile = malloc(sizeof(SDL_Rect*) * map->rows);
    for (int i = 0; i < map->rows; i++) {
        map -> tile[i] = malloc(sizeof(SDL_Rect) * map->cols);
        for (int j = 0; j < map->cols; j++) {
            map->tile[i][j].h = SIZE;
            map->tile[i][j].w = SIZE;
            map->tile[i][j].x = 10 + SIZE * j;  //Should move over a column
            map->tile[i][j].y = 10 + SIZE * i;  //Should go down a row
        }
    }
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

void displayMap(Map* map, SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer, &map->tile[map->avtrRow][map->avtrCols]);
    SDL_SetRenderDrawColor(renderer, 0,59,111,255);
    for (int i = 0; i < map->rows; i++) {
        SDL_RenderDrawRects(renderer, map->tile[i], map->cols);
    }
}

/**
 * 
 * @param map The current level the avatar is moving around on
 * @param axis Should either be an 'x' or 'y' other inputs are not recognized
 *                  'x' (for moving the avatar in the same row) 
 *                  'y' (for moving the avatar in the same col)
 * @param spaces How many tiles the avatar is moving along said axis
 *                  negative for up or left
 *                  positive for down or right
 */
void moveAvtr(Map* map, SDL_Renderer* renderer, char axis, int spaces){
    
    //Setting the current position to a normal tile
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer, &map->tile[map->avtrRow][map->avtrCols]);
    SDL_SetRenderDrawColor(renderer, 0,59,111,255);
    SDL_RenderDrawRect(renderer, &map->tile[map->avtrRow][map->avtrCols]);

    //Setting the new position to the avatar tile
    if (axis == 'x'){
        map->avtrRow += spaces;
    } else if (axis =='y'){
        map->avtrCols += spaces;
    }
    
    //Drawing the new position of the avatar
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer, &map->tile[map->avtrRow][map->avtrCols]);
}

