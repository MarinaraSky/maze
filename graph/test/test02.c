#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphSerializer.h"

int main(void)
{
    FILE *fp = fopen("sample/test02", "r");
    if (!fp)
    {
        perror("Unable to open sample/test02");
        return 1;
    }

    Graph *network = GraphSerializer_fromFile(fp);

    if (!network)
    {
        fprintf(stderr, "Unable to deserialize sample/test02\n");
        fclose(fp);
        return 1;
    }

    Graph_deleteNode(network, "MD");

    Graph_print(network);

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
