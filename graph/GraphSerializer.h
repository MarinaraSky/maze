#ifndef GRAPHSERIALIZER_H
 #define GRAPHSERIALIZER_H


/**
 * @brief Print out serialized version of graph
 * @param g Graph object to print
 */
void
GraphSerializer_toStdout(const Graph *g);

/**
 * @brief Deserializes Graph object from input
 * @param fp FILE * to read serialized Graph from
 * @return Graph object (NULL on error)
 */
Graph *
GraphSerializer_fromFile(FILE *fp);

#endif
