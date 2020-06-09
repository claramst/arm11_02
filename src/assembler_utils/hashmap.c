#include "hashmap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

Map *createMap(void) {
    Map *map = malloc(sizeof(Map));
    map->front = NULL;
    return map;
}

void freeNode(Node *node) {
//    free(node);
}

void freeMap(Map *map) {
    Node *elem = map->front;
    Node *next;
    while (elem != NULL) {
        next = elem->next;
        freeNode(elem);
        elem = next;
    }
}

Node *createNode(char *label, int value) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->key = label;
    node->val = value;
    node->next = NULL;
    return node;
}

int getValue(Map *map, char *label) {
    for (Node *curr = map->front; curr; curr = curr->next) {
      printf("1\n");
      printf("%s\n",curr->key);
      printf("%s\n", label);
        if (strcmp(curr->key, label) == 0) {
          printf("returning\n");
            return curr->val;
        }
    }
    return -1;
}

//Adds node at the start of the map
void addNode(Map *map, char *label, int value) {
    Node *node = createNode(label, value);
    node->next = map->front;
    map->front = node;
}

void addNodes(Map *map, char *keys[], int values[], int n) {
    for (int i = 0; i < n; i++) {
        addNode(map, keys[i], values[i]);
    }
}
