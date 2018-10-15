#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Dijkstra.h"
#include "graph/Graph.h"
#include "graph/GraphSerializer.h"

/**
 * @brief Generates 2d array of a maze
 * @param customMaze char *** to modify into a maze
 * @param maxLineLength Size passed in on CLI
 * @param lineCount Size passed in on CLI
 */
void
Maze_generate(char ***customMaze, size_t maxLineLength, size_t lineCount);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <map>\n", argv[0]);
        return 1;
    }

	char flags = 0;
	size_t size = 0;
	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-d") == 0)
		{
			flags |= 1;
		}
		else if(strcmp(argv[i], "-w") == 0)
		{
			flags |= 2;
		}
		else if(strcmp(argv[i], "-m") == 0)
		{
			flags |= 4;
		}
		else if((flags &= 4) == 4 && strcmp(argv[i - 1], "-m") == 0)
		{
			size = strtol(argv[i], NULL, 10);
			if(!size)
			{
				flags ^= 4;	
			}
		}
	}
	char **mazeToSolve = NULL;
	size_t maxLineLength = 0;
	size_t lineCount = 0;
	Graph *network = NULL;
	
	if(((flags & 4) == 4) && size > 5)
	{
		maxLineLength = size;
		lineCount = size;
		Maze_generate(&mazeToSolve, maxLineLength, lineCount);
		network = Graph_create();
		GraphSerializer_fromMaze(&network, mazeToSolve, &maxLineLength, &lineCount, flags);
    	if (!network)
    	{
        	fprintf(stderr, "Unable to load graph");
        	return 1;
		}
	}

	else
	{
		FILE *fp = fopen(argv[1], "r");
		if (!fp)
		{
			perror("Unable to open file");
			return 1;
		}

		network = GraphSerializer_fromFile(fp, &mazeToSolve, &maxLineLength, &lineCount, flags);
    	if (!network)
    	{
        	fprintf(stderr, "Unable to load graph");
        	return 1;
    	}

		fclose(fp);
	}

    char **route;
	char start[2] = ">";
	char end[2] = "@";

    ssize_t hops = Dijkstra_path(network, start, end, &route);
	Dijkstra_solveMaze(mazeToSolve, route, hops);
	for(size_t i = 0; i < lineCount; i++)
	{
		for(size_t j = 0; j < maxLineLength + 1; j++)
		{
			printf("%c", mazeToSolve[i][j]);
			if(mazeToSolve[i][j] == '\n')
			{
				break;
			}
		}
		free(mazeToSolve[i]);
	}
	free(mazeToSolve);


    free(route);
    Graph_disassemble(network);
	return 0;
}
