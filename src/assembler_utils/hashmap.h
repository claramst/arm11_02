#ifndef HASHMAP_H
#define HASHMAP_H

/**
 * Header file for hashmap.
 * contains the struct definiton for node and map.
 * contains function declarations for each hashmap function.
 */

typedef struct node {
  char *key;
  int val;
  struct node *next;
} Node;

typedef struct map {
  Node *front;
} Map;

Map *create_map(void);
Node *create_node(char *label, int value);
int get_value(Map *map, char *label);
void add_node(Map *map, char *label, int value);
void free_node(Node *node);
void free_map(Map *map);
void add_nodes(Map *map, char **label, int *value, int n);

#endif
