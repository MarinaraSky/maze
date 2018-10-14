#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphSerializer.h"

int main(void)
{
    FILE *fp = fopen("sample/alfa", "r");
    if (!fp)
    {
        perror("Unable to open sample/alfa");
        return 1;
    }

    Graph *network = GraphSerializer_fromFile(fp);
    fclose(fp);

    if (!network)
    {
        fprintf(stderr, "Unable to deserialize sample/alfa\n");
        return 1;
    }

    Graph_print(network);

    Graph_disassemble(network);
}
