#ifndef MAP_H
 #define MAP_H

#include <stdbool.h>

// Uses strings for keys and doubles for values
// key may not be NULL

typedef struct _map map;


map *map_create(void);

bool map_insert(map *m, const char *key, double value);

bool map_exists(map *m, const char *key);

double map_lookup(map *m, const char *key);

void map_destroy(map *m);

#endif
