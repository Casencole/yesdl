// Created by casen on 2/20/2024.
// I wanted this to be a general use singley linked list, but now it's kinda to intergrated for this program,
// but I could make a copy of it for something new and change what is needed.
// Anyway this is a singley linked list I feel like I don't need to explain more than that to be hoenst 
#include <stdlib.h>
#include "sll.h"

/**
 * Helper function for insertNode just creates the node and allcoates memory for it
 * @param name String that will be stored in the node
 * @return the new node with the name stored
 */
Buttons* createNode(char* name){
    Buttons* newNode = malloc(sizeof(Buttons));
    if (newNode == NULL){
        SDL_Log("newNode was not made\n");
        return 0;
    }
    newNode->name = malloc(strlen(name) + 1);
    if (newNode->name == NULL){
        SDL_Log("newNode.name was not made\n");
        return 0;
    }
    newNode->rect = malloc(sizeof(SDL_Rect));
    newNode->textRect = malloc(sizeof(SDL_Rect));
    strcpy(newNode->name, name);
    newNode->selected = 0;
    newNode->txr = NULL;
    newNode->next = NULL;
    return newNode ;
}

/**
 * Inserts a node at the end of a linked list
 * @param head The head node of a linked list
 * @param name String that will be stored in the node
 */
void insertNode(Buttons** head, char* name){
    Buttons* newNode = createNode(name);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Buttons* temp = *head;
        while (temp->next != NULL) {
            temp = (Buttons *) temp->next;
        }
        temp->next = newNode;
    }
}

/**
 * removes a node from a linked list and fixes the pointer of the nodes around it to accommodate 
 * @param head The head node of a linked list
 * @param key right now the key is just a string is compared to the name
 *              (only works if each node has a unique name which in this application it should)
 */
void removeNode(Buttons** head, const char* key){
    Buttons* curent = *head;
    Buttons* prev = NULL;
    
    if (curent != NULL && strcmp(curent->name, key) == 0){
        *head = (Buttons *) curent->next;
        free(curent);
        return;
    }
    
    while (curent != NULL && strcmp(curent->name, key) != 0){
        prev = curent;
        curent = (Buttons *) curent->next;
    }
    
    if (curent == NULL) return;
    prev->next = curent->next;
    free(curent);
}

/**
 * Frees all the nodes in the list from the head and onward
 * @param head The head node of a linked list
 */
void destroyList(Buttons** head){
    Buttons* current = *head;
    Buttons* prev = NULL;
    while (current != NULL){
        prev = current;
        SDL_DestroyTexture(current->txr);
        current = (Buttons *) current->next;
        free(prev->name);
        free(prev->textRect);
        free(prev->rect);
        free(prev);
    }
}

/**
 * Prints the whole list from the node given onward (does not technicly need to be a head)
 * @param head The head node of a linked list
 */
void printList(Buttons* head){
    Buttons *current = head;
    SDL_Log("~~~~~~~~~~~~~~~~~~~~~~~~~\nINFO: Current Levels Selected\n");
    while (current != NULL) {
        SDL_Log("%s\n", current->name);
        current = (Buttons *) current->next;
    }
    SDL_Log("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

/**
 * Checks the mouse position and checks to see if it is inside a node
 * @param head The head node of a linked list
 * @param x mouse x postion
 * @param y mouse y postion
 * @return  if in the node returns said node
 *          otherwise reuturns null if not in a node
 */
Buttons** isInNode(Buttons** head, int x, int y){
    Buttons** current = head;
    while (*current != NULL) {
        if (x > (*current)->rect->x &&
            x < (*current)->rect->x + (*current)->rect->w &&
            y > (*current)->rect->y &&
            y < (*current)->rect->y + (*current)->rect->h) {
            SDL_Log("Clicked Buttons %s", (*current)->name);
            return current;
        }
        current = (Buttons**) &((*current)->next);
    }
    return NULL;
}

/**
 * Will change all the boxes y value and by "change" amount should be 1 or -1 but will leave as int incase I want to
 * use it for more or greater changes later
 * @param head The head node of a linked list
 * @param change The amount you want to change the y axis of the node
 * @return 0 if move went well
 *        -1 if move not made
 */
int updatePos(Buttons** head, int change){
    // Check to make sure there is enough buttons to need to scroll
    int i = 0;
    Buttons* current = *head;
    while (current != NULL){
        current = (Buttons *) current->next;
        i++;
    }
    if (i < 15){// Need at least 15 buttons to scroll
        return -1;
    }
    
    current = *head;
    while (current != NULL) {
        // Loop through eveything and update new position
        current->rect->y += change;
        current->textRect->y += change;
        
        // Make sure you can't scroll down past the last one
        if (current->next == NULL && current->rect->y < (360 - current->rect->h)){
            updatePos(head, 1);
        }
        
        current = (Buttons *) current->next;
    }
    
    // Make sure you can't scroll past the first one
    if ( (*head)->rect->y > 10){
        updatePos(head, -1);
    }
    return -1;
}


