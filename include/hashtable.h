
typedef struct ht_node {
    char *key;
    char *value;
    struct ht_node *next;
} ht_node;

ht_node *new_node(char *key, char *value);
void free_node(ht_node *node);