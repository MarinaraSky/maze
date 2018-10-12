#ifndef PQUEUE_H
 #define PQUEUE_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct _pqueue pqueue;


pqueue *pqueue_create(int (*cmp)(void *, void *));

pqueue *pqueue_build(const void **data, size_t sz, int (*cmp)(void *, void *));

size_t pqueue_size(const pqueue *pq);

bool pqueue_enqueue(pqueue *pq, void *datum);

void *pqueue_dequeue(pqueue *pq);

bool pqueue_search(const pqueue *pq, void *needle);

void pqueue_disassemble(pqueue *pq);

#endif
