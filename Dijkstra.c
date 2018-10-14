#include "Dijkstra.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "map/map.h"
#include "map/vmap.h"
#include "dpqueue/pqueue.h"


ssize_t Dijkstra_path(const Graph *g, const char *start, const char *end, char ***path)
{
    if (!g || !start || !end || !path)
    {
        errno = EINVAL;
        return -1;
    }
    *path = NULL;

    map *distance = map_create();
    if (!distance)
    {
        errno = ENOMEM;
        return -1;
    }
    vmap *prev = vmap_create();
    if (!prev)
    {
        map_destroy(distance);
        errno = ENOMEM;
        return -1;
    }
    pqueue *next = pqueue_create((int (*)(void *, void *))strcmp);
    if (!next)
    {
        vmap_destroy(prev);
        map_destroy(distance);
        errno = ENOMEM;
        return -1;
    }

    // TODO ABC
    map_insert(distance, start, 0);
    pqueue_enqueue(next, (void *)start, 0);
    vmap_insert(prev, start, NULL);

    while (pqueue_size(next) > 0)
    {
        char *curr;
        double currDistance = pqueue_dequeue(next, (void **)&curr);

        char **neighbors;
        ssize_t neighborCount = Graph_getNeighbors(g, curr, &neighbors);
        if (neighborCount < 0)
        {
            goto cleanup;
        }

        for (ssize_t n=0; n < neighborCount; ++n)
        {
            double weight = Graph_getEdgeWeight(g, curr, neighbors[n]);
            double currBestDistanceToNeighbor = map_lookup(distance, neighbors[n]);

            if (isnan(currBestDistanceToNeighbor))
            {
                // TODO ABC
                vmap_insert(prev, neighbors[n], curr);
                map_insert(distance, neighbors[n], currDistance + weight);
                pqueue_enqueue(next, neighbors[n], currDistance + weight);
            }
            else if (currDistance + weight < currBestDistanceToNeighbor)
            {
                vmap_insert(prev, neighbors[n], curr);
                map_insert(distance, neighbors[n], currDistance + weight);
                pqueue_reprioritize(next, neighbors[n], currDistance + weight);
            }
        }

        free(neighbors);
    }

    const char *pos = end;
    ssize_t hops = 0;
    while (pos)
    {
        ++hops;
        pos = vmap_lookup(prev, pos);
    }

    char **results = malloc(hops * sizeof(*results));
    if (!results)
    {
        hops = -1;
        goto cleanup;
    }

    pos = end;
    for (ssize_t n=hops-1; n >= 0; --n)
    {
        results[n] = (char *)pos;
        pos = vmap_lookup(prev, pos);
    }

    *path = results;

cleanup:
    pqueue_destroy(next);
    map_destroy(distance);
    vmap_destroy(prev);

    return hops;
}

void Dijkstra_solveMaze(char **mazeFromFile, char **route, size_t hops)
{
	char letters[82] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890-_=+!$%^&*(){}[]|`~";
	size_t yCoord = 0;
	size_t xCoord = 0;
	for(size_t i = 0; i < hops; i++)
	{
		for(size_t j = 0; j < sizeof(letters); j++)
		{
			if(route[i][1] == letters[j])
			{
				yCoord = j;
			}
			if(route[i][2] == letters[j])
			{
				xCoord = j;
			}
			if(yCoord != 0 && xCoord != 0)
			{
				mazeFromFile[yCoord][xCoord] = '.';
				continue;
			}
		}
	}
}
