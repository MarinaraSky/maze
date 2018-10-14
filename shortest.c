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

	char **mazeFromFile = NULL;
	size_t maxLineLength = 0;
	size_t lineCount = 0;
    Graph *network = GraphSerializer_fromFile(fp, &mazeFromFile, &maxLineLength, &lineCount);

    fclose(fp);

    if (!network)
    {
        // TODO Give more context/reason why this happened
        fprintf(stderr, "Unable to load graph");
        return 1;
    }

    char **route;
	char start[2] = ">";
	char end[2] = "@";

    ssize_t hops = Dijkstra_path(network, start, end, &route);
	Dijkstra_solveMaze(mazeFromFile, route, hops);
	for(size_t i = 0; i < lineCount; i++)
	{
		for(size_t j = 0; j < maxLineLength; j++)
		{
			printf("%c", mazeFromFile[i][j]);
			if(mazeFromFile[i][j] == '\n')
			{
				break;
			}
		}
	}


    free(route);
    Graph_disassemble(network);
	return 0;
}
