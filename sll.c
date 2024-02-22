//
// Created by casen on 2/20/2024.
//
#include <stdlib.h>
#include "sll.h"
#include "SDL2/SDL.h"

Node* createNode(char* name){
    Node* newNode;
    if (newNode == NULL){
        SDL_Log("newNode was not made\n");
        return 0;
    }
    newNode->name = name;
    newNode->next = NULL;
    return newNode ;
}

void insertNode(Node** head, char* name){
    Node* newNode = createNode(name);
    if (head == NULL) {
        head = &newNode;
        return;
    }
    else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        return;
    }
}

//Need to make this not a mess and fix it still
void removeNode(Node** head, int data){
    Node* curent = *head;
    Node* prev = malloc(sizeof(Node));
    
    while (curent->data != data){
        prev = curent;
        curent = curent->next;
    }
    prev->next = curent->next;
    free(curent);
}

void printList(Node* head){
    
}