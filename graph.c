// 315318766 Omer Bar
// 209549542 Tomer Peisikov
#include "graph.h"

node * createNode(vertex v) {
    node *newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->v = v;
    newNode->next = NULL;
    return newNode;
}

void addEdge(Graph *graph, vertex source, vertex destination) {
    node *newNode = createNode(destination);
    newNode->next = graph->adjacencyLists[source];
    graph->adjacencyLists[source] = newNode;

    // source point to new edge therefore updating the number of egdes source point to
    graph->adjacencyNumber[source] += 1;

    // all the vertices that point to source
    node *pointerNode = createNode(source);
    node *t = graph->pointerToMe[destination];
    pointerNode->next = t;
    graph->pointerToMe[destination] = pointerNode;
}

Graph * createGraph(int vertices) {
    Graph *graph = malloc(sizeof(Graph));
    // total number of vertices
    graph->numVertices = vertices;

    // array[i] is a linked list of which verticies vertex i points to
    graph->adjacencyLists = malloc(vertices * sizeof(node *));

    // array[i] the number of verticies vertex i points to
    graph->adjacencyNumber = malloc(vertices*sizeof(int));

    // array[i] linkeded list of all the verticeies that points to i
    graph->pointerToMe = malloc(vertices * sizeof(node *));

    // unused don't delete
    graph->numVisits = malloc(vertices*sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->adjacencyLists[i] = NULL;
        graph->adjacencyNumber[i] = 0;
        graph->pointerToMe[i] = NULL;
        graph->numVisits[i] = 0;
    }
    return graph;
}


