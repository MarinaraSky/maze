#include "pqueue.h"


#include <math.h>
#include <stdio.h>
#include <string.h>

struct _pqueue {
	void **data;
	size_t size;
	size_t capacity;
	int (*cmp)(void *, void *);
};

// "Bubbles down" a value through the heap
static void bubble_down(struct _pqueue *pq, size_t idx)
{
	while(2*idx + 1 < pq->size) {
		size_t left = 2*idx + 1;
		size_t right = 2*idx + 2;
		size_t to_swap = idx;

		if(pq->cmp(pq->data[left], pq->data[to_swap]) < 0) {
			to_swap = left;
		}
		if(right < pq->size &&
				pq->cmp(pq->data[right], pq->data[to_swap]) < 0) {
			to_swap = right;
		}

		if(to_swap == idx) {
			break;
		}

		void *tmp = pq->data[idx];
		pq->data[idx] = pq->data[to_swap];
		pq->data[to_swap] = tmp;

		idx = to_swap;
	}
}

pqueue *pqueue_create(int (*cmp)(void *, void *))
{
	struct _pqueue *pq = malloc(sizeof(*pq));
	if(!pq) {
		return NULL;
	}

	pq->size = 0;
	pq->capacity = 4;
	pq->cmp = cmp;
	pq->data = malloc(pq->capacity * sizeof(*pq->data));
	if(!pq->data) {
		free(pq);
		return NULL;
	}

	return pq;
}

pqueue *pqueue_build(const void **data, size_t sz, int (*cmp)(void *, void *))
{
	if(!data) {
		return NULL;
	}
	if(sz == 0) {
		return pqueue_create(cmp);
	}

	struct _pqueue *pq = malloc(sizeof(*pq));
	if(!pq) {
		return NULL;
	}

	pq->capacity = sz;
	pq->size = sz;
	pq->data = malloc(pq->capacity * sizeof(*pq->data));
	if(!pq->data) {
		free(pq);
		return NULL;
	}
	memcpy(pq->data, data, sz * sizeof(*data));

	size_t current_root = (pq->size - 2) / 2;
	while(current_root > 0) {
		bubble_down(pq, current_root);
		--current_root;
	}

	bubble_down(pq, 0);

	return pq;
}

size_t pqueue_size(const pqueue *pq)
{
	if(!pq) {
		return 0;
	}
	return pq->size;
}

bool pqueue_enqueue(pqueue *pq, void *datum)
{
	if(!pq) {
		return false;
	}

	// Double the capacity if needed
	if(pq->size == pq->capacity) {
		void *tmp = realloc(pq->data,
				2 * pq->capacity * sizeof(*pq->data));
		if(!tmp) {
			return false;
		}
		pq->capacity *= 2;
		pq->data = tmp;
	}

	size_t current_idx = pq->size;
	pq->size++;
	pq->data[current_idx] = datum;

	size_t parent_idx = (current_idx - 1) / 2;
	while(current_idx > 0) {
		// Exit the loop if the heap property is satisfied
		if(pq->cmp(pq->data[parent_idx], pq->data[current_idx]) < 1) {
			break;
		}
		// Swap the parent and current values
		void *tmp = pq->data[parent_idx];
		pq->data[parent_idx] = pq->data[current_idx];
		pq->data[current_idx] = tmp;

		// Move up the heap
		current_idx = parent_idx;
		parent_idx = (current_idx - 1) / 2;
	}

	return true;
}

void *pqueue_dequeue(pqueue *pq)
{
	if(!pq || pq->size == 0) {
		return NULL;
	}

	void *value = pq->data[0];
	pq->size--;

	pq->data[0] = pq->data[pq->size];

	bubble_down(pq, 0);

	return value;
}

bool pqueue_search(const pqueue *pq, void *needle)
{
	if(!pq) {
		return false;
	}
	for(size_t n=0; n < pq->size; ++n) {
		if(pq->cmp(pq->data[n], needle) == 0) {
			return true;
		}
	}

	return false;
}

void pqueue_disassemble(pqueue *pq)
{
	free(pq);
}


