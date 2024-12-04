#include <hashtable.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_ht_add_and_get() {
    char *key = "key%d";
    char *value = "value%d";

    h_table *table = ht_new();

    for (size_t i = 0; i < 100; i++) {
        size_t size = 20;
        char key_buf[size];
        char val_buf[size];
        snprintf(key_buf, size, key, i);
        snprintf(val_buf, size, value, i);
        ht_put(table, key_buf, val_buf);

        char *result = ht_get(table, key_buf);
        assert(0 == strcmp(result, val_buf));
    }

    ht_free(table);
}

int main(void) {
    test_ht_add_and_get();
}