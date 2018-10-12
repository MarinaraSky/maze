#define _XOPEN_SOURCE 500
#include "Graph.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct graph {
    char **nodes;
    size_t size;
    double *weights;
};

inline static size_t
idx(const Graph *g, size_t src, size_t dst)
{
    return src*g->size + dst;
}

Graph *
Graph_create(void)
{
    Graph *g = malloc(sizeof(*g));
    if (!g)
    {
        return NULL;
    }

    g->size = 0;
    g->weights = NULL;
    g->nodes = NULL;

    return g;
}

bool
Graph_addNode(Graph *g, const char *name)
{
    if (!g || !name)
    {
        return false;
    }

    for (size_t n=0; n < g->size; ++n)
    {
        if (strcmp(g->nodes[n], name) == 0)
        {
            return false;
        }
    }

    char **tmp = realloc(g->nodes, (g->size + 1) * sizeof(*g->nodes));
    if (!tmp)
    {
        return false;
    }

    g->nodes = tmp;
    g->nodes[g->size] = strdup(name);
    if (!g->nodes[g->size])
    {
        return false;
    }
    g->size++;

    double *newWeights = malloc(g->size * g->size * sizeof(*newWeights));
    if (!newWeights)
    {
        g->size--;
        free(g->nodes[g->size]);
        return false;
    }

    //TODO: Iterate in reverse which means a realloc can be used
    for (size_t y=0; y < g->size - 1; ++y)
    {
        for (size_t x=0; x < g->size - 1; ++x)
        {
            // g has already changed sizes, so can't use idx() function
            // for original index
            newWeights[idx(g, y, x)] = g->weights[(g->size-1)*y + x];
        }
    }
    for (size_t n=0; n < g->size; ++n)
    {
        newWeights[idx(g, n, g->size-1)] = NAN;
        newWeights[idx(g, g->size-1, n)] = NAN;
    }

    free(g->weights);
    g->weights = newWeights;

    return true;
}

bool
Graph_addEdge(Graph *g, const char *from, const char *to, double weight)
{
    if (!g || !from || !to || isnan(weight))
    {
        return false;
    }

    ssize_t srcIdx=-1, dstIdx=-1;
    //TODO Check each iteration through to see if both indices have been found
    for (size_t n=0; n < g->size; ++n)
    {
        if (strcmp(g->nodes[n], from) == 0)
        {
            srcIdx = n;
        }
        if (strcmp(g->nodes[n], to) == 0)
        {
            dstIdx = n;
        }
    }
    if (srcIdx == -1 || dstIdx == -1)
    {
        return false;
    }

    g->weights[idx(g, srcIdx, dstIdx)] = weight;

    return true;
}

bool
Graph_isAdjacent(const Graph *g, const char *from, const char *to)
{
    return !isnan(Graph_getEdgeWeight(g, from, to));
}

ssize_t
Graph_getNodes(const Graph *g, char ***nodes)
{
    if (!g || !nodes)
    {
        errno = EINVAL;
        return -1;
    }

    size_t byteCount = g->size * sizeof(*g->nodes);
    if (byteCount == 0)
    {
        *nodes = NULL;
        return 0;
    }

    char **results = malloc(byteCount);
    if (!results)
    {
        errno = ENOMEM;
        *nodes = NULL;
        return -1;
    }

    memcpy(results, g->nodes, byteCount);
    *nodes = results;
    return g->size;
}

ssize_t
Graph_getNeighbors(const Graph *g, const char *name, char ***neighbors)
{
    if (!g || !name || !neighbors)
    {
        errno = EINVAL;
        return -1;
    }

    ssize_t srcIdx=-1;
    //TODO Check each iteration through to see if both indices have been found
    for (size_t n=0; n < g->size; ++n)
    {
        if (strcmp(g->nodes[n], name) == 0)
        {
            srcIdx = n;
            break;
        }
    }
    if (srcIdx == -1)
    {
        //TODO Should errno be set to indicate this entry doesn't exist?
        *neighbors = NULL;
        return 0;
    }

    // TODO Only allocate enough for the neighbors that exist
    size_t byteCount = g->size * sizeof(*g->nodes);
    if (byteCount == 0)
    {
        *neighbors = NULL;
        return 0;
    }

    char **results = malloc(byteCount);
    if (!results)
    {
        errno = ENOMEM;
        *neighbors = NULL;
        return -1;
    }

    size_t neighborCount = 0;
    for (size_t e=0; e < g->size; ++e)
    {
        if (!isnan(g->weights[idx(g, srcIdx, e)]))
        {
            results[neighborCount++] = g->nodes[e];
        }
    }

    *neighbors = results;
    return neighborCount;
}

double
Graph_getEdgeWeight(const Graph *g, const char *from, const char *to)
{
    if (!g || !from || !to)
    {
        return NAN;
    }

    ssize_t srcIdx=-1, dstIdx=-1;
    //TODO Check each iteration through to see if both indices have been found
    for (size_t n=0; n < g->size; ++n)
    {
        if (strcmp(g->nodes[n], from) == 0)
        {
            srcIdx = n;
        }
        if (strcmp(g->nodes[n], to) == 0)
        {
            dstIdx = n;
        }
    }
    if (srcIdx == -1 || dstIdx == -1)
    {
        return NAN;
    }

    return g->weights[idx(g, srcIdx, dstIdx)];
}

void
Graph_deleteNode(Graph *g, const char *name)
{
    if (!g || !name)
    {
        return;
    }

    ssize_t srcSearch=-1;
    for (size_t n=0; n < g->size; ++n)
    {
        if (strcmp(g->nodes[n], name) == 0)
        {
            srcSearch = n;
            break;
        }
    }
    if (srcSearch < 0)
    {
        return;
    }
    size_t srcIdx = srcSearch;

    double *newWeights = malloc(g->size * g->size * sizeof(*newWeights));
    if (!newWeights)
    {
        return;
    }

    free(g->nodes[srcIdx]);
    // Copy latter part of array over the removed index
    memmove(g->nodes + srcIdx, g->nodes + (srcIdx + 1), (g->size - srcIdx - 1) * sizeof(*g->nodes));

    for (size_t y=0; y < g->size; ++y)
    {
        size_t newX, newY;
        newY = y;
        if (y == srcIdx)
        {
            continue;
        }
        else if (y > srcIdx)
        {
            --newY;
        }

        for (size_t x=0; x < g->size; ++x)
        {
            newX = x;
            if (x == srcIdx)
            {
                continue;
            }
            else if (x > srcIdx)
            {
                --newX;
            }

            newWeights[(g->size-1)*newY + newX] = g->weights[idx(g, y, x)];
        }
    }

    g->size--;

    free(g->weights);
    g->weights = newWeights;
}

void
Graph_deleteEdge(Graph *g, const char *from, const char *to)
{
    if (!g || !from || !to)
    {
        return;
    }

    ssize_t srcIdx=-1, dstIdx=-1;
    //TODO Check each iteration through to see if both indices have been found
    for (size_t n=0; n < g->size; ++n)
    {
        if (strcmp(g->nodes[n], from) == 0)
        {
            srcIdx = n;
        }
        if (strcmp(g->nodes[n], to) == 0)
        {
            dstIdx = n;
        }
    }
    if (srcIdx == -1 || dstIdx == -1)
    {
        return;
    }

    g->weights[idx(g, srcIdx, dstIdx)] = NAN;
}

void
Graph_print(const Graph *g)
{
    for (size_t n=0; n < g->size; ++n)
    {
        printf("%s: ", g->nodes[n]);

        for (size_t e=0; e < g->size; ++e)
        {
            if (!isnan(g->weights[idx(g, n, e)]))
            {
                printf("→ %s ", g->nodes[e]);
            }
        }
        puts("");
    }

}

void
Graph_disassemble(Graph *g)
{
    if (!g)
    {
        return;
    }

    for (size_t n=0; n < g->size; ++n)
    {
        free(g->nodes[n]);
    }
    free(g->nodes);
    free(g->weights);

    free(g);
}
