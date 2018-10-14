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
 * @param mazeFromFile char *** to make the map read be available in main
 * @param maxLineLength Tracking for mazeFromFile size (X axis)
 * @param lineCount Tracking for mazeFromFile size (Y axis)
 * @return Graph object (NULL on error)
 */
Graph *
GraphSerializer_fromFile(FILE *fp, char ***mazeFromFile, size_t *maxLineLength, size_t *lineCount);

#endif
