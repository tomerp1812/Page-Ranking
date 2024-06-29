// 315318766 Omer Bar
// 209549542 Tomer Peisikov
// Created by omer on 6/16/24.
//

#ifndef EX2PARLLEL_SOLUTION_H
#define EX2PARLLEL_SOLUTION_H

#define MAX_THREADS 4
#define MIN_THREADS 2
#define CACHE_SIZE 8

#include "graph.h"
#include <stdio.h>
#include <string.h>

typedef struct PageWithoutLink {
    int index;
    struct PageWithoutLink* next;
} PageWithoutLink ;

void PageRank(Graph* g, int n, float* rank);
// void PageRankSq(Graph* g, int n, float* rank);

#endif //EX2PARLLEL_SOLUTION_H