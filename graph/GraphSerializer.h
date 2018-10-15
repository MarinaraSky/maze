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
 * @param flags Used to keep track of CLI arguments
 * @return Graph object (NULL on error)
 */
Graph *
GraphSerializer_fromFile(FILE *fp, char ***mazeFromFile, size_t *maxLineLength, size_t *lineCount, char flags);

/**
 * @brief Taking a 2d array will make nodes from each available space
 * @param g Graph ** to modify and produce graph from maze
 * @param mazeFromFile char *** to parse and make the graph
 * @param maxLineLength Tracking for mazeFromFile size (X axis)
 * @param lineCount Tracking for mazeFromFile size (Y axis)
 * @param flags Used to keep track of CLI arguments
 */
void 
GraphSerializer_fromMaze(Graph **g, char **maze, size_t *maxLength, size_t *lineCount, char flags);

#endif
