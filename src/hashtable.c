#include <sys/types.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

#define INIT 5381
#define HASHTABLE_CAPACITY 16

uint32_t hash_f(char *s) {
    uint32_t hash = INIT;
    char c;
    while((c = *s++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

uint32_t get_idx(char *s, uint32_t m) {
    return hash_f(s) % m;
}

typedef struct ht_node {
    char *key;
    char *value;
    struct ht_node *next;
} ht_node;

ht_node *ht_node_new(char *key, char *value) {
    ht_node *node = (ht_node*) malloc(sizeof(ht_node));
    node->key = (char*) malloc(strlen(key));
    node->value = (char*) malloc(strlen(value));
    strcpy(node->key, key);
    strcpy(node->value, value);
    return node;
}

void ht_node_free(ht_node *node) {
    free(node->key);
    free(node->value);
    free(node);
}

typedef struct h_table {
    int cap;
    int size;
    ht_node **table;
} h_table;

h_table *ht_new(void) {
    h_table *ht = (h_table*) malloc(sizeof(h_table));
    ht->cap = HASHTABLE_CAPACITY;
    ht->size = 0;
    ht->table = (ht_node**) calloc(ht->cap, sizeof(ht_node*));
    memset(ht->table, 0, ht->cap);
    return ht;
}

void ht_free(h_table *ht) {
    for (int i = 0; i < ht->cap; i++) {
        ht_node *node = ht->table[i];
        while (node != NULL) {
            ht_node *temp = node;
            node = node->next;
            ht_node_free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

void ht_expand_and_rehash(h_table *ht) {
    int cap = ht->cap * 2;
    ht_node **table = (ht_node**) malloc(cap * sizeof(ht_node));
    memset(table, 0, cap * sizeof(ht_node));
    for (int i = 0; i < ht->cap; i++) {
        ht_node *node = ht->table[i];
        while (node != NULL) {
            ht_node *next = node->next;
            int index = get_idx(node->key, cap);
            node->next = table[index];
            table[index] = node;
            node = next;
        }
    }
    free(ht->table);
    ht->table = table;
    ht->cap = cap;
}

void ht_put(h_table *ht, char *key, char *value) {
    if (ht->size == ht->cap) {
        ht_expand_and_rehash(ht);
    }
    
    int index = get_idx(key, ht->cap);

    ht_node *curr = ht->table[index];
    while (curr != NULL) {
        if (0 == strcmp(curr->key, key)) {
            curr->value = (char*) realloc(curr->value, strlen(value));
            strcpy(curr->value, value);
            return;
        } else if (curr->next == NULL) {
            curr->next = ht_node_new(key, value);
            ht->size++;
            return;
        }
        curr = curr->next;
    }
    ht->table[index] = ht_node_new(key, value);
    ht->size++;
}

char *ht_get(h_table *ht, char *key) {
    int index = get_idx(key, ht->cap);
    ht_node *node = ht->table[index];
    while (node != NULL) {
        if (0 == strcmp(node->key, key)) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

void ht_remove(h_table *ht, char *key) {
    int index = get_idx(key, ht->cap);
    ht_node *curr = ht->table[index];
    ht_node *prev = NULL;

    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (prev) {
                prev->next = curr->next;
            } else {
                ht->table[index] = curr->next;
            }
            free(curr->key);
            free(curr->value);
            free(curr);
            ht->size--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}