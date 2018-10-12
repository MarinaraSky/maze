#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphSerializer.h"

int main(void)
{
    FILE *fp = fopen("sample/test03", "r");
    if (!fp)
    {
        perror("Unable to open sample/test03");
        return 1;
    }

    Graph *network = GraphSerializer_fromFile(fp);

    if (!network)
    {
        fprintf(stderr, "Unable to deserialize sample/test03\n");
        fclose(fp);
        return 1;
    }

    char **results;
    ssize_t resultsCount = Graph_getNeighbors(network, "MD", &results);
    if (resultsCount < 0)
    {
        perror("Unable to get neighbors");
        fclose(fp);
        Graph_disassemble(network);
        return 1;
    }

    for (ssize_t n = 0; n < resultsCount; ++n)
    {
        puts(results[n]);
    }

    rewind(fp);

    char buf[256];
    while (fgets(buf, sizeof(buf), fp))
    {
         if (buf[0] == '#')
         {
             printf("%s", buf);
         }
    }

    fclose(fp);

    Graph_disassemble(network);
}
