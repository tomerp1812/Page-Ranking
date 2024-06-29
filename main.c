#include <stdio.h>
#include "solution.h"

void printGraph(Graph *graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        node *temp = graph->adjacencyLists[i];
        printf("Adjacency list of vertex %d\n", i);
        while (temp) {
            printf("%d -> ", temp->v);
            temp = temp->next;
        }
        printf("NULL\n");
    }

    for (int i = 0; i < graph->numVertices; i++) {
        node *temp = graph->pointerToMe[i];
        printf("Pointer to me list of vertex %d\n", i);
        while (temp) {
            printf("%d -> ", temp->v);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

void printArrayWithIndicesSS(float *array, int size) {
    for (int i = 0; i < size; i++) {
        printf("INDEX %d: RANK = %f\n", i, array[i]);
    }
}


int main() {
    // int N = 3; // number of nodes
    // int iterations = 100; // number of iterations
    //
    // // Initialize the graph
    // Graph *graph = createGraph(N);
    //
    // // Example graph structure
    // addEdge(graph, 0, 1);
    // addEdge(graph, 0, 2);
    // addEdge(graph, 1, 0);
    // addEdge(graph, 1, 2);
    // addEdge(graph, 2, 0);

    srand(time(0));

    int numVertices = 500;
    int numOfIteration = 100;
    printf("starting --- creating graph with %d vertices\n", numVertices);
    Graph *graph = createGraph(numVertices);

    // Randomly add edges between vertices
    for (int i = 0; i < numVertices / 2; i++) {
        // Random number of edges for each vertex
        int flag = rand() % 2;
        if (flag == 0) continue;
        int numEdges = rand() % (numVertices / 2);

        for (int j = 0; j < numEdges; j++) {
            vertex destination = rand() % numVertices;
            if (destination != i) {
                int flag = 1;
                node* t = graph->adjacencyLists[i];
                while(t != NULL) {
                    if (t->v == destination) {
                        flag = 0;
                        break;
                    }
                    t = t->next;
                }
                if (flag) {
                    addEdge(graph, i, destination);
                }
            }
        }
    }
    printf("finish --- creating graph with %d vertices\n", numVertices);

    clock_t start, end;
    double cpu_time_used;
    float* res = malloc(sizeof(float)*graph->numVertices);

    // Parrllel
    start = clock();
    PageRank(graph, numOfIteration, res);
    end = clock();
    printArrayWithIndicesSS(res, graph->numVertices);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by %s: %f seconds\n\n", "PageRank", cpu_time_used);


    // // Sequntial
    // start = clock();
    // PageRankSq(graph, iterations, res2);
    // end = clock();
    // printArrayWithIndicesSS(res2, graph->numVertices);
    //
    // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("Time taken by %s: %f seconds\n\n", "PageRankSequntial", cpu_time_used);

    free(res);

    return 0;

}
