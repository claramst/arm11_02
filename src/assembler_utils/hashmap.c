#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hashmap.h"
#include "../global_utils/errorhandling.h"

/**
 * hashmap.c contains functions for creating our hash map data structure, freeing the map, creating a node, adding a node to the map,* adding multiple nodes to the map at once, and getting the value
 * associated with a key in our map.
 *
 */

/**
 * createMap allocates memory for an instance of our Map structure
 */
Map *create_map(void) {
  Map *map = malloc(sizeof(Map));
  CHECK_PRED(!map, "Error allocating memory for map");
  map->front = NULL;
  return map;
}

/**
 * freeNode frees memory allocated for the given node
 */
void free_node(Node *node) {
  free(node);
}

/**
 * freeMap frees the given map and all its nodes
 */
void free_map(Map *map) {
  assert(map);
  for (Node *elem = map->front, *next; elem; elem = next) {
	next = elem->next;
	freeNode(elem);
  }
  free(map);
}

/**
 * createNode allocates memory for a node and assigns it the given
 * key and value.
 */
Node *create_node(char *key, int value) {
  Node *node = (Node *) malloc(sizeof(Node));
  CHECK_PRED(!node, "Error allocating memory for a hashmap node");
  node->key = key;
  node->val = value;
  node->next = NULL;
  return node;
}

/**
 * getValue returns the value associated with the given key in
 * the given Map. It ensures the key given is not NULL, and
 * if
 *
 */
int get_value(Map *map, char *key) {
  assert(key != NULL);
  for (Node *curr = map->front; curr; curr = curr->next) {
	if (strcmp(curr->key, key) == 0) {
	  return curr->val;
	}
  }
  return -1;
}

/**
 * Creates a new node and adds it to the front of the specified map.
 */
void add_node(Map *map, char *label, int value) {
  Node *node = createNode(label, value);
  node->next = map->front;
  map->front = node;
}

/**
 * Analogous to addNode, however multiple nodes can be added at once.
 * Note that keys[] and values[] must both be of length n.
 * @param n The number of nodes to be added.
 */
void add_nodes(Map *map, char *keys[], int values[], int n) {
  for (int i = 0; i < n; i++)
	addNode(map, keys[i], values[i]);
}
