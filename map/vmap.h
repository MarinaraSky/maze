#ifndef VMAP_H
 #define VMAP_H

#include <stdbool.h>

// key may not be NULL

typedef struct _vmap vmap;


vmap *vmap_create(void);

bool vmap_insert(vmap *m, const char *key, void *value);

bool vmap_exists(vmap *m, const char *key);

void *vmap_lookup(vmap *m, const char *key);

void vmap_destroy(vmap *m);

#endif
