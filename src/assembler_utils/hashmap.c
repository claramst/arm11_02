#include "hashmap.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

Map *createMap(void) {
  Map *map = malloc(sizeof(Map));
  //check pred
  map->front = NULL;
  return map;
}

void freeNode(Node *node) {
  free(node);
}

void freeMap(Map *map) {
  assert(map);
//  Node *elem = map->front;
//  Node *next;
  for (Node *elem = map->front, *next; elem; elem = next) {
	next = elem->next;
	freeNode(elem);
  }
//  while (elem != NULL) {
//	next = elem->next;
//	freeNode(elem);
//	elem = next;
//  }
  free(map);
}

//todo: put CHECK_PRED somewhere that all files can use it
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
  for (int i = 0; i < n; i++)
	addNode(map, keys[i], values[i]);
}

int countNodes(Map *map) {
  assert(map);
  int count = 0;
  for (Node *elem = map->front, *next; elem; elem = next) {
	next = elem->next;
	count++;
  }
  return count;
}
