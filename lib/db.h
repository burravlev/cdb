#ifndef db_h
#define db_h

#include <string.h>

#include "../lib/hashtable.h"

void db_init();
void db_close();
char *read_cmd(char *s);

#endif