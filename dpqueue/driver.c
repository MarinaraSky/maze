#include <stdio.h>
#include <string.h>

#include "pqueue.h"

// Exists in heap.o, but not in pqueue.h
void            heap_print(
    const pqueue * pq);


int
main(
    void)
{
    pqueue         *list = pqueue_create((int (*)(void *, void *)) strcmp);

    puts("Created priority queue");
    heap_print(list);

    pqueue_enqueue(list, "Alfa", 17.0);
    heap_print(list);
    pqueue_enqueue(list, "Bravo", 18.0);
    heap_print(list);

    pqueue_enqueue(list, "Charlie", 10.0);
    heap_print(list);

    pqueue_enqueue(list, "Delta", 8.6);
    heap_print(list);

    pqueue_enqueue(list, "Eagle", 7.5);
    heap_print(list);

    pqueue_enqueue(list, "Foxtrot", 3.09);
    heap_print(list);

    pqueue_reprioritize(list, "Delta", 1.09);
    heap_print(list);

    pqueue_reprioritize(list, "Eagle", 109);
    heap_print(list);

    char           *name;
    double          value = pqueue_dequeue(list, (void **) &name);

    printf("Top priority is %lf: %s\n", value, name);
    heap_print(list);

    value = pqueue_dequeue(list, (void **) &name);
    printf("Top priority is %lf: %s\n", value, name);
    heap_print(list);

    value = pqueue_dequeue(list, (void **) &name);
    printf("Top priority is %lf: %s\n", value, name);
    heap_print(list);

    value = pqueue_dequeue(list, (void **) &name);
    printf("Top priority is %lf: %s\n", value, name);
    heap_print(list);

    value = pqueue_dequeue(list, (void **) &name);
    printf("Top priority is %lf: %s\n", value, name);
    heap_print(list);

    value = pqueue_dequeue(list, (void **) &name);
    printf("Top priority is %lf: %s\n", value, name);
    heap_print(list);

    value = pqueue_dequeue(list, (void **) &name);
    printf("Top priority is %lf: %s\n", value, name);
    heap_print(list);

    pqueue_destroy(list);
}
