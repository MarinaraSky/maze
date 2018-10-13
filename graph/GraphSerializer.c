#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"

void
GraphSerializer_toStdout(const Graph *g)
{
	char **nodes;
	ssize_t nodeCount = Graph_getNodes(g, &nodes);
	if (nodeCount < 0)
	{
		perror("Unable to get nodes");
		return;
	}

	for (ssize_t n=0; n < nodeCount; ++n)
	{
		char **edges;
		ssize_t edgeCount = Graph_getNeighbors(g, nodes[n], &edges);
		if (edgeCount < 0)
		{
			perror("Unable to get edges");
			fprintf(stderr, "Edges from %s missing\n", nodes[n]);
			continue;
		}

		for (ssize_t e=0; e < edgeCount; ++e)
		{
			printf("%s %s %lf\n", nodes[n], edges[e], Graph_getEdgeWeight(g, nodes[n], edges[e]));
		}

		free(edges);
	}

	free(nodes);
}

Graph *
GraphSerializer_fromFile(FILE *fp)
{
	Graph *g = Graph_create();
	if (!g)
	{
		return NULL;
	}

	
	char *buf = NULL;
	size_t sizeOfBuf = 1;
	size_t prevLineLength = 0;
	size_t nextLineLength = 0;
	size_t maxLength = 0;
	size_t lineCount = 0;

	getline(&buf, &sizeOfBuf, fp);
	prevLineLength = sizeOfBuf;
	rewind(fp);


	while(getline(&buf, &sizeOfBuf, fp) != -1)
	{
		size_t curLineSize = sizeOfBuf;
		char *curLine = strdup(buf);
		long loc = ftell(fp);
		getline(&buf, &sizeOfBuf, fp);
		rewind(fp);
		nextLineLength = sizeOfBuf;
		fseek(fp, loc, SEEK_SET);
		if(curLineSize < prevLineLength && curLineSize < nextLineLength)
		{
			printf("invalid map\n");
			return NULL;
		}	
		if(curLineSize > maxLength)
		{
			maxLength = strlen(curLine);
		}

		lineCount++;
		prevLineLength = curLineSize;
		sizeOfBuf = 1;
		free(curLine);
	}
	rewind(fp);
	
	char **maze = malloc(lineCount * sizeof(char*));
	for(size_t i = 0; i < lineCount; i++)
	{
		maze[i] = calloc(maxLength + 1, 1);
	}
	size_t lineInMaze = -1;
	sizeOfBuf = 1;
	while(getline(&buf, &sizeOfBuf, fp) != -1)
	{
		strcpy(maze[++lineInMaze], buf);
		sizeOfBuf = 1;
	}

	for(size_t i = 0; i < lineCount; i++)
	{
		for(size_t j = 0; j < maxLength; j++)
		{
			printf("%c", maze[i][j]);
		}
	}
	char letters[82] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890-_=+!$%^&*(){}[]|`~";
	for(size_t i = 0; i < lineCount; i++)
	{
		for(size_t j = 0; j < maxLength; j++)
		{
			if(maze[i][j] != '#' && maze[i][j] != '\n')
			{ 
				char *curr = malloc(4);
				if(maze[i][j] == '@' || maze[i][j] == '>')
				{
					curr[0] = maze[i][j];
					curr[1] = '\0';
				}
				else
				{
					curr[0] = letters[maze[i][j] % sizeof(letters)];
					curr[1] = letters[i % sizeof(letters)];
					curr[2] = letters[j % sizeof(letters)];
					curr[3] = '\0';
				}
				Graph_addNode(g, curr);
				if(i < lineCount - 1 && maze[i + 1][j] != '#')
				{
					char *next = malloc(4);
					if(maze[i + 1][j] == '@' || maze[i + 1][j] == '>')
					{
						next[0] = maze[i + 1][j];
						next[1] = '\0';
					}
					else
					{
						next[0] = letters[maze[i + 1][j] % sizeof(letters)];
						next[1] = letters[(i + 1) % sizeof(letters)];
						next[2] = letters[j % sizeof(letters)];
						next[3] = '\0';
					}
					Graph_addNode(g, next);
					Graph_addEdge(g, curr, next, 1.0);
					free(next);
				}
				if(i > 0 && maze[i - 1][j] != '#')
				{
					char *next = malloc(4);
					if(maze[i - 1][j] == '@' || maze[i - 1][j] == '>')
					{
						next[0] = maze[i - 1][j];
						next[1] = '\0';
					}
					else
					{
						next[0] = letters[maze[i - 1][j] % sizeof(letters)];
						next[1] = letters[(i - 1) % sizeof(letters)];
						next[2] = letters[j % sizeof(letters)];
						next[3] = '\0';
					}
					Graph_addNode(g, next);
					Graph_addEdge(g, curr, next, 1.0);
					free(next);
				}
				if(j < maxLength  - 1 && maze[i][j + 1] != '#')
				{ 
					char *next = malloc(4);
					if(maze[i][j + 1] == '@' || maze[i][j + 1] == '>')
					{
						next[0] = maze[i][j + 1];
						next[1] = '\0';
					}
					else
					{
						next[0] = letters[maze[i][j + 1] % sizeof(letters)];
						next[1] = letters[i % sizeof(letters)];
						next[2] = letters[(j + 1) % sizeof(letters)];
						next[3] = '\0';
					}
					Graph_addNode(g, next);
					Graph_addEdge(g, curr, next, 1.0);
					free(next);
				}
				if(j > 0 && maze[i][j - 1] != '#')
				{
					char *next = malloc(4);
					if(maze[i][j - 1] == '@' || maze[i][j - 1] == '>')
					{
						next[0] = maze[i][j - 1];
						next[1] = '\0';
					}
					else
					{
						next[0] = letters[maze[i][j - 1] % sizeof(letters)];
						next[1] = letters[i % sizeof(letters)];
						next[2] = letters[(j - 1) % sizeof(letters)];
						next[3] = '\0';
					}
					Graph_addNode(g, next);
					Graph_addEdge(g, curr, next, 1.0);
					free(next);
				}
				free(curr);
			}	
		}
	}

	free(buf);
	for(size_t i = 0; i < lineCount; i++)
	{
		free(maze[i]);
	}
	free(maze);

	/*(
		// TODO This is inefficient, it may be better to do a single scan
		// to a Map for nodes, then re-scan the file to add edges.
		Graph_addNode(g, from);
		Graph_addNode(g, to);
		Graph_addEdge(g, from, to, weight);

		++lineNumber;
		*/
	

	return g;
}

