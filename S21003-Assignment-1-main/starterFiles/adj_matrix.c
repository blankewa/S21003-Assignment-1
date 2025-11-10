//so many comments

/**
 * FILE: ADJ_MATRIX.C
 * 
 * THIS FILE  CONTAINS EMPTY / DO-NOTHING IMPLEMENTATIONS
 * FOR THE FUNCTIONS REQUIRED FOR THE ADJACENCY MATRIX STRUCT. 
 *
 * DO NOT RENAME THESE FUNCTIONS OR CHANGE THEIR RETURN
 * TYPES OR INPUT PARAMETERS.
 *
 * YOU NEED TO COMPLETE THE IMPLEMENTATION OF THESE
 * FUNCTIONS. 
 *
 * THESE FUNCTIONS RECEIVE INPUT PARAMETERS THAT WILL
 * PROVIDE THEM WITH THE INFORMATION THEY REQUIRE. 
 *
 * THE FUNCTIONS RETURN VALUES TO REPRESENT SUCCESS, ERROR,
 * OR, IN SOME CASES, DATA THAT HAS TO BE RETURNED. THE CALLER 
 * OF THESE FUNCTIONS WILL EXPECT TO RECEIVE THESE RETURN VALUES 
 * IN ORDER TO VERIFY THE OUTCOME OF THE FUNCTION CALL. 
 *
 * IF THERE ARE ANY FUNCTIONS THAT YOU DON'T HAVE TIME TO 
 * IMPLEMENT, PLEASE LEAVE THE EMPTY / DO-NOTHING IMPLEMENTATION
 * FOR THEM IN THIS FILE, OTHERWISE YOUR PROGRAM WON'T COMPILE
 * WHEN IT IS FED INTO OUR TESTER PROGRAM.
 * 
 */

/** 
 * ### INCLUDES
 */
#include "my_malloc.h"          // gives access to memory allocation functions, including 'myMalloc'
#include <stdio.h>              // required for access to file input-output functions in C
#include "adj_matrix.h"         // required, to include the Adjacency Matrix data structures and function declarations
                                // that are being implemented in this file
#include <stdbool.h>            // required, to include the definition of the 








//createAdjacencyMatrix func, creates an adjacency matrix w/ default edge value
AdjacencyMatrix* createAdjacencyMatrix(int defaultEdgeValue)
{
    AdjacencyMatrix *m = myMalloc(sizeof *m);
    if (!m) return NULL;

    m->defaultEdgeValue = defaultEdgeValue;
    m->numVertices = NUMBER_OF_VERTICES;
    m->numEdges = 0;

    m->matrix = myMalloc(NUMBER_OF_VERTICES * sizeof(int *));
    if (!m->matrix) { free(m); return NULL; }

    for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
        m->matrix[i] = myMalloc(NUMBER_OF_VERTICES * sizeof(int));
        if (!m->matrix[i]) {
            for (int k = 0; k < i; ++k) free(m->matrix[k]);
            free(m->matrix);
            free(m);
            return NULL;
        }
        for (int j = 0; j < NUMBER_OF_VERTICES; ++j)
            m->matrix[i][j] = defaultEdgeValue;
    }
    return m;
}

//addEdge func, adds an edge to the adjacency matrix
int addEdge(AdjacencyMatrix *pMatrix, int src, int dest, int weight)
{
    if (!pMatrix || src < 0 || src >= pMatrix->numVertices ||
        dest < 0 || dest >= pMatrix->numVertices) {
        return INVALID_INPUT_PARAMETER;
    }

    int prev = pMatrix->matrix[src][dest];
    pMatrix->matrix[src][dest] = weight;

    if (prev == pMatrix->defaultEdgeValue && weight != pMatrix->defaultEdgeValue) {
        pMatrix->numEdges++;
    } else if (prev != pMatrix->defaultEdgeValue && weight == pMatrix->defaultEdgeValue) {
        if (pMatrix->numEdges > 0) pMatrix->numEdges--;
    }

    return SUCCESS;
}

//addEdges func, adds several edges to matrix
int addEdges(AdjacencyMatrix *pMatrix, Edge edges[], int edgeNum)
{ 
    if (!pMatrix || !edges || edgeNum <= 0) {
        return INVALID_INPUT_PARAMETER;
    }

    int validEdgeCount = 0;
    int invalidEdgeCount = 0;

    for (int i = 0; i < edgeNum; i++) {
        int src = edges[i].src;
        int dest = edges[i].dest;
        int weight = edges[i].weight;

        if (src < 0 || src >= pMatrix->numVertices || 
            dest < 0 || dest >= pMatrix->numVertices) {
            invalidEdgeCount++;
            continue;
        }

        if (addEdge(pMatrix, src, dest, weight) == SUCCESS) {
            validEdgeCount++;
        } else {
            invalidEdgeCount++;
        }
    }

    if (validEdgeCount == 0) {
        return INVALID_INPUT_PARAMETER;
    } else if (invalidEdgeCount > 0) {
        return PARTIAL_SUCCESS;
    }

    return SUCCESS;
}

//loadMatrixFromFile func, loads adjacency matrix from probably tester (hopefully it still works)
int loadMatrixFromFile(AdjacencyMatrix *pMatrix, char filename[])
{
    if (!pMatrix || !filename) {
        return INVALID_INPUT_PARAMETER;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        return FILE_NOT_FOUND;
    }

    for (int i = 0; i < pMatrix->numVertices; i++) {
        for (int j = 0; j < pMatrix->numVertices; j++) {
            if (fscanf(file, "%d", &pMatrix->matrix[i][j]) != 1) {
                return FILE_READ_ERROR;
            }
        }
    }

    return SUCCESS;
}



//doDepthFirstTraversal func, performs depth first traversal on matrix
int doDepthFirstTraversal(AdjacencyMatrix *pMatrix, int startingNode, int traversalOutput[])
{ 
    if (!pMatrix || startingNode < 0 || startingNode >= pMatrix->numVertices || !traversalOutput) {
        return INVALID_INPUT_PARAMETER;
    }

    bool visited[NUMBER_OF_VERTICES] = { false };
    int stack[NUMBER_OF_VERTICES];
    int top = -1;
    int outputIndex = 0;

    stack[++top] = startingNode;

    while (top >= 0) {
        int currentNode = stack[top--];

        if (!visited[currentNode]) {
            visited[currentNode] = true;
            traversalOutput[outputIndex++] = currentNode;

            for (int i = 0; i < pMatrix->numVertices; i++) {
                if (pMatrix->matrix[currentNode][i] != pMatrix->defaultEdgeValue && !visited[i]) {
                    stack[++top] = i;
                }
            }
        }
    }

    return SUCCESS;
}