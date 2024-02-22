//
// Created by casen on 2/20/2024.
//

#ifndef YESDL_SLL_H
#define YESDL_SLL_H
//Shoule 1000% generalize this at somepoint, so I don't need to change data types for diffrent projects

typedef struct {
    char* name;
    void* next;
} Node ;
//int data is just a place-holder type that needs to change to be something more unqiue
Node* createNode(char* name);             //Helper function to insert
void insertNode(Node** head, char* name); //Adds the node at the end of the linked list
void removeNode(Node** head, char* name);
void printList(Node*); //This one is purley for dev purposes at least the way I am using it when I make it

#endif //YESDL_SLL_H
