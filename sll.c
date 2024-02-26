//
// Created by casen on 2/20/2024.
//
#include <stdlib.h>
#include "sll.h"


Node* createNode(char* name){
    Node* newNode = malloc(sizeof(Node));
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
    strcpy(newNode->name, name);
    newNode->selected = 0;
    newNode->next = NULL;
    return newNode ;
}

void insertNode(Node** head, char* name){
    Node* newNode = createNode(name);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = (Node *) temp->next;
        }
        temp->next = newNode;
    }
}

//Need to make this not a mess and fix it still
void removeNode(Node** head, const char* key){
    Node* curent = *head;
    Node* prev = NULL;
    
    if (curent != NULL && strcmp(curent->name, key) == 0){
        *head = (Node *) curent->next;
        free(curent);
        return;
    }
    
    while (curent != NULL && strcmp(curent->name, key) != 0){
        prev = curent;
        curent = (Node *) curent->next;
    }
    
    if (curent == NULL) return;
    prev->next = curent->next;
    free(curent);
}

void destroyList(Node** head){
    Node* curent = *head;
    Node* prev = NULL;
    while (curent != NULL){
        prev = curent;
        curent = (Node *) curent->next;
        free(prev->name);
        free(prev->rect);
        free(prev);
    }
}

void printList(Node* head){
    Node *current = head;
    SDL_Log("~~~~~~~~~~~~~~~~~~~~~~~~~\nINFO: Current Levels Selected\n");
    while (current != NULL) {
        SDL_Log("%s\n", current->name);
        current = (Node *) current->next;
    }
    SDL_Log("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

Node** isInNode(Node** head, int x, int y){
    Node** current = head;
    int i = 0 ;
    while (*current != NULL) {
        if (x > (*current)->rect->x &&
            x < (*current)->rect->x + (*current)->rect->w &&
            y > (*current)->rect->y &&
            y < (*current)->rect->y + (*current)->rect->h) {
            SDL_Log("Clicked Node %s", (*current)->name);
            return current;
        }
        //SDL_Log("test %d", i++);
        current = (Node **) &((*current)->next);
    }
    return NULL;
}