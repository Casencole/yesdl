// Created by Casen on 2/20/2024.
// I wanted this to be a general use singly linked list, but now it's integrated too much into this project,
// but I could make a copy of it for something new and change what is needed.
// Anyway this is a singly linked list I feel like I don't need to explain more than that to be honest
#include <cstdlib>
#include <string>
#include "sll.h"

/**
 * Helper function for insertNode just creates the node and allocates memory for it
 * @param name String that will be stored in the node
 * @return the new node with the name stored
 */
Button* createNode(std::string name){
    auto *newNode = new Button;
    newNode->name = name;
    newNode->rect = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    newNode->textRect =(SDL_Rect*) malloc(sizeof(SDL_Rect));
    newNode->name = name;
    newNode->selected = 0;
    newNode->txr = nullptr;
    newNode->next = nullptr;
    SDL_Log("Created Node: %s", newNode->name.c_str());
    return newNode ;
}

/**
 * Inserts a node at the end of a linked list
 * @param head The head node of a linked list
 * @param name String that will be stored in the node
 */
void insertNode(Button** head, std::string name){
    Button* newNode = createNode(name);
    if (*head == nullptr) {
        *head = newNode;
    } else {
        Button* temp = *head;
        while (temp->next != nullptr) {
            temp = temp->next;
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
void removeNode(Button** head, std::string key){
    Button* current = *head;
    Button* prev = nullptr;

    if (current != nullptr && current->name == key){
        *head = current->next;
        delete(current);
        return;
    }

    while (current != nullptr && current->name != key){
        prev = current;
        current = current->next;
    }

    if (current == nullptr) return;
    prev->next = current->next;
    delete(current);
}

/**
 * Frees all the nodes in the list from the head and onward
 * @param head The head node of a linked list
 */
void destroyList(Button** head){
    Button* current = *head;
    Button* prev;
    while (current != nullptr){
        prev = current;
        SDL_DestroyTexture(current->txr);
        current = current->next;
        free(prev->textRect);
        free(prev->rect);
        delete(prev);
    }
}

/**
 * Empties a list
 * @param head The head of a linked list
 */
void clearList(Button** head){
    Button* current = *head;
    while (current != nullptr) {
        removeNode(head, current->name);
        current = current->next;
    }
}

/**
 * Deselects all buttons in a list
 * @param head The head of a linked list
 */
void deselectAllButtons(Button* head){
    Button* current = head;
    while (current != nullptr) {
        current->selected = 0;
        current = current->next;
    }
}


/**
 * Prints the whole list from the node given onward (does not technically need to be a head)
 * @param head The head node of a linked list
 */
void printList(Button* head){
    Button *current = head;
    SDL_Log("~~~~~~~~~~~~~~~~~~~~~~~~~\nINFO: Current Levels Selected\n");
    while (current != nullptr) {
        SDL_Log("%s\n", current->name.c_str());
        current = current->next;
    }
    SDL_Log("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

/**
 * Checks the mouse position and checks to see if it is inside a node
 * @param head The head node of a linked list
 * @param x mouse x position
 * @param y mouse y position
 * @return  if in the node returns said node
 *          otherwise returns null if not in a node
 */
Button* isInNode(Button* head, int x, int y){
    Button* current = head;
    while (current != nullptr) {
        if (x > current->rect->x &&
            x < current->rect->x + current->rect->w &&
            y > current->rect->y &&
            y < current->rect->y + current->rect->h) {
            SDL_Log("Clicked Button %s", current->name.c_str());
            return current;
        }
        current = (current->next);
    }
    return nullptr;
}

/**
 * Will change all the boxes y value and by "change" amount should be 1 or -1 but will leave as int in case I want to
 * use it for more or greater changes later
 * @param head The head node of a linked list
 * @param change The amount you want to change the y axis of the node
 * @return 0 if move went well
 *        -1 if move not made
 */
int updatePos(Button** head, int change){
    // Check to make sure there is enough buttons to need to scroll
    int i = 0;
    Button* current = *head;
    while (current != nullptr){
        current = current->next;
        i++;
    }
    if (i < 15){// Need at least 15 buttons to scroll
        return -1;
    }
    
    current = *head;
    while (current != nullptr) {
        // Loop through everything and update new position
        current->rect->y += change;
        current->textRect->y += change;
        
        // Make sure you can't scroll down past the last one
        if (current->next == nullptr && current->rect->y < (360 - current->rect->h)){
            updatePos(head, 1);
        }
        
        current = current->next;
    }
    
    // Make sure you can't scroll past the first one
    if ( (*head)->rect->y > 10){
        updatePos(head, -1);
    }
    return 0;
}


