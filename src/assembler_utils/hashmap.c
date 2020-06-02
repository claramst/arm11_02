#include "hashmap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

typedef struct node {
  char *key;
  int val;
  struct node *next;
} Node;

typedef struct map {
  Node *front;
} Map;

Map *createMap(void) {
  Map *map = malloc(sizeof(Map));
  map->front = NULL;
  return map;
} 

Node *createNode(char *label, int value) {
  Node *node = (Node *) malloc(sizeof(Node));
  node->key = label;
  node->val = value;
  node->next = NULL;
  return node;
}

int getValue(Map map, char *label) {
  for (Node *curr = map.front; curr; curr = curr->next) {
    if (strcmp(curr->key, label) == 0) {
      return curr->val;
    }
  }
}

//Adds node at the start of the map
void addNode(Map *map, Node *node) {
  node->next = map->front;
  map->front = node;
}

