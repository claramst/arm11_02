#include "hashmap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>

Map *createMap(void) {
    Map *map = malloc(sizeof(Map));
    map->front = NULL;
    return map;
}

void freeNode(Node *node) {
    free(node);
}

void freeMap(Map *map) {
    Node *elem = map->front;
    Node *next;
    while (elem != NULL) {
        next = elem->next;
        freeNode(elem);
        elem = next;
    }
    free(map);
}

Node *createNode(char *label, int value) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->key = label;
    node->val = value;
    node->next = NULL;
    return node;
}

int getValue(Map *map, char *label) {
  assert(label != NULL);
    for (Node *curr = map->front; curr; curr = curr->next) {
        if (strcmp(curr->key, label) == 0) {
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
