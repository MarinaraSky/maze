#include <stdio.h>
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

	char buf[128];
	size_t lineNumber = 1;

	while (fgets(buf, sizeof(buf), fp))
	{
		size_t len = strlen(buf);
		if (len == sizeof(buf)-1 && buf[len-1] != '\n')
		{
			fprintf(stderr, "Line %zu starting with '%s' too long\n", lineNumber, buf);
			fclose(fp);
			Graph_disassemble(g);
			return NULL;
		}

		char *from = strtok(buf, " \t\f\v\r\n");
		char *to = strtok(NULL, " \t\f\v\r\n");
		char *sWeight = strtok(NULL, " \t\f\v\r\n");
        // Skip comment lines
        if (buf[0] == '#')
        {
			++lineNumber;
			continue;
        }
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
	}

	return g;
}

