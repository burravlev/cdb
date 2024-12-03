#include <hashtable.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void) {
    char *key = "key";
    char *value = "value";

    ht_node *node = new_node(key, value);

    assert(strcmp(node->key, key) == 0);
    assert(strcmp(node->value, value) == 0);
}