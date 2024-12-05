#ifndef hashtable_h
#define hashtable_h

#include <stddef.h>

typedef struct ht_node {
    char *key;
    char *value;
    struct ht_node *next;
} ht_node;

ht_node *ht_node_new(char *key, char *value);
void ht_node_free(ht_node *node);

typedef struct h_table {
    size_t cap;
    size_t size;
    ht_node **table;
} h_table;

h_table *ht_new(void);
void ht_free(h_table *ht);

void ht_put(h_table *ht, char *key, char *value);
char *ht_get(h_table *ht, char *key);
void ht_remove(h_table *ht, char *key);

#endif