#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct node {
  char *key;
  int val;
  struct node *next;
} Node;

typedef struct map {
  Node *front;
} Map;

Map *createMap(void);
Node *createNode(char *label, int value);
int getValue(Map *map, char *label);
void addNode(Map *map, char *label, int value);
void freeNode(Node *node);
void freeMap(Map *map);
void addNodes(Map *map, char **label, int *value, int n);
int countNodes(Map *map);
#endif

