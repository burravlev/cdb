#ifndef db_h
#define db_h

#include <string.h>
#include <hashtable.h>

char *read_cmd(h_table *table, const char *s);

#endif