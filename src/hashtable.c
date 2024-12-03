#include <hashtable.h>
#include <stdlib.h>
#include <string.h>

ht_node *new_node(char *key, char *value) {
    ht_node *node = (ht_node*) malloc(sizeof(ht_node));
    node->key = malloc(strlen(key));
    node->value = malloc(strlen(value));
    strcpy(node->key, key);
    strcpy(node->value, value);
    return node;
}

void free_node(ht_node *node) {
    free(node->key);
    free(node->value);
    free(node);
}