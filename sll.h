//
// Created by casen on 2/20/2024.
//
#include "SDL2/SDL.h"
#include <string.h>
#ifndef YESDL_SLL_H
#define YESDL_SLL_H
//Shoule 1000% generalize this at somepoint, so I don't need to change data types for diffrent projects

typedef struct {
    char* name;
    int selected;
    SDL_Rect* rect;
    struct Node* next;
} Node ;
//int data is just a place-holder type that needs to change to be something more unqiue
Node* createNode(char*);             //Helper function to insert
void insertNode(Node** head, char*); //Adds the node at the end of the linked list
void removeNode(Node** head, const char*);
void destroyList(Node** head);
void printList(Node*); //This one is purley for dev purposes at least the way I am using it when I make it
Node** isInNode(Node** head, int x, int y);

#endif //YESDL_SLL_H
