#include <hashtable.h>
#include <string.h>
#include <db.h>
#include <stdio.h>

char *OK_STATUS = "OK";

char *read_cmd(h_table *ht, const char *s) {
    char cmd[50];
    char key[50];
    char value[50];
    int parsed = sscanf(s, "%s %s %s", cmd, key, value);
    if (parsed == 3 && strcmp(cmd, "set") == 0) {
        ht_put(ht, key, value);
        return OK_STATUS;
    } else if (parsed == 2 && strcmp(cmd, "get") == 0) {
        char *result = ht_get(ht, key);
        if (result == NULL) {
            return NULL;
        }
        return result;
    } else if (parsed == 2 && strcmp(cmd, "del") == 0) {
        ht_remove(ht, key);
        return OK_STATUS;
    } else if (parsed == 1 && strcmp(cmd, "exit") == 0) {
        printf("exit\n");
        return "exit";
    }
    return "OK";
}