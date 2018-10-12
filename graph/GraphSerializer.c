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
	free(buf);
	/*
	for(size_t i = 0; i < lineCount; i++)
	{
		for(size_t j = 0; j < maxLength; j++)
		{
			printf("%c", maze[i + j]);
		}
	}
	*/
		/*
		char *from = strtok(buf, " \t\f\v\r\n");
		char *to = strtok(NULL, " \t\f\v\r\n");
		char *sWeight = strtok(NULL, " \t\f\v\r\n");
		if (!from || !to || !sWeight)
		{
			fprintf(stderr, "Line %zu invalid format, skipping\n", lineNumber);
			++lineNumber;
			continue;
		}

		char *err;
		double weight = strtod(sWeight, &err);
		if (*err)
		{
			fprintf(stderr, "Line %zu invalid weight, skipping\n", lineNumber);
			++lineNumber;
			continue;
		}

		// TODO This is inefficient, it may be better to do a single scan
		// to a Map for nodes, then re-scan the file to add edges.
		Graph_addNode(g, from);
		Graph_addNode(g, to);
		Graph_addEdge(g, from, to, weight);

		++lineNumber;
		*/
	

	return NULL;
}

