#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "graph/Graph.h"
#include "graph/GraphSerializer.h"


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("Unable to open file");
        return 1;
    }

    Graph *network = GraphSerializer_fromFile(fp);

    fclose(fp);

    if (!network)
    {
        // TODO Give more context/reason why this happened
        fprintf(stderr, "Unable to load graph");
        return 1;
    }

	/*
    char **route;

    ssize_t hops = Dijkstra_path(network, argv[2], argv[3], &route);

    printf("Path is %zd hops long\n", hops);
    for (ssize_t n=0; n < hops; ++n)
    {
        printf(" %s → ", route[n]);
    }
    puts("");

    free(route);
    Graph_disassemble(network);
	*/
	free(network);
	return 0;
}
