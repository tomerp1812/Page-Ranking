// 315318766 Omer Bar
// 209549542 Tomer Peisikov
// Created by omer on 6/16/24.
//
#include "solution.h"
#include "thr_pool.h"
#define DAMPING_FACTOR 0.15

struct calculatePageWithOutLinkArgs {
    int *adjacencyNumber;
    int start;
    int end;
    PageWithoutLink *result;
};

void *calculatePageWithOutLink(void *arg){
    struct calculatePageWithOutLinkArgs *args = arg;
    if (args) {
        for (int i = args->start; i < args->end; i++) {
            if (args->adjacencyNumber[i] == 0) {
                PageWithoutLink* t = malloc(sizeof(PageWithoutLink));
                if (t == NULL) {
                    printf("failed to malloc PageWithoutLink\n");
                    exit(1);
                }
                t->index = i;
                t->next = args->result;
                args->result = t;
            }
        }
    } else {
        printf("calculatePageWithOutLinkArgs is null\n");
        exit(1);
    }
}


struct rankCalculationArgsV2 {
    float dn;
    float minusD;
    const float* rankBefore;
    node** pointerToMe;
    int* adjacencyNumber;
    float rankUJDivN;
    int N;
    int start;
    int end;
    float *res;
    struct calculatePageWithOutLinkArgs* PageWithoutLinkArgs;
    int PageWithoutLinkArgsSize;
};

/**
 * Calculate a chunk of verticies rank, for each v in the chunk will hold value in relative *res to be the
 * rank at stage i+1, doing the calculation by rank of stage i (rankBefore argument)
 * @param arg - rankCalculationArgs*
 * @return
 */
void *rankCalculationV2(void *arg){
    struct rankCalculationArgsV2 *args = arg;
    /*
    *       float dn;               // consts
            float minusD;           // consts
            float* rankBefore;      // odd/even
            node** pointerToMe;     // array of node linkedlist
            int* adjacencyNumber;   // the number of outlink each vertice had
            int N;                  // size of verticie
            int start;              // start of offset
            int end;                // end of offset
            float *res;             // result array.
     */
    if (args) {
        // calculate rankUJ
        float total = 0;
        for (int i = 0; i < args->PageWithoutLinkArgsSize; i++) {
            PageWithoutLink* t = args->PageWithoutLinkArgs[i].result;
            while(t != NULL) {
                total += args->rankBefore[t->index];
                t = t->next;
            }
        }
        args->rankUJDivN = total / args->N;
        float sumRankLeft;
        node* pointerToMe;
        int* adjacencyNumber = args->adjacencyNumber;
        for(int j = args->start; j < args->end; j++) {
            sumRankLeft = 0;
            pointerToMe = args->pointerToMe[j];
            while(pointerToMe != NULL) {
                sumRankLeft += args->rankBefore[pointerToMe->v] / adjacencyNumber[pointerToMe->v];
                pointerToMe = pointerToMe->next;
            }
            total = sumRankLeft + args->rankUJDivN;
            total = args->dn + args->minusD*total;
            args->res[j] = total;
        }
    }
}

/**
 * Return the sum of element in array
 * @param array - float*
 * @param size - int
 * @return
 */
float sumArray(const float* array, const int size) {
    float total = 0.0f;
    for (int i = 0; i < size; i++) {
        total += array[i];
    }
    return total;
}

/**
 * Parallel PageRanking algorithm with defined number of thread to be max - 4 and min - 2
 * Benchmark:
 * Graph with 2000 Verticeis and each one with at most 100 edges.
 * Parrllel -  6 sec
 * Sequntial - 350 sec
 * @param g - Graph
 * @param n - Number of iteration
 * @param rank - Pointer to the result array
 */
void PageRank(Graph* g, int n, float* rank) {
    /*
     * Graph Struct
     *
     * n - number of iteration
     * result in *rank
     * */

    // Initializing thread-pool
    thr_pool_t* thr = thr_pool_create(MIN_THREADS, MAX_THREADS, 60, NULL);
    if (thr == NULL) {
        printf("failed to create thr_pool_create()\n");
        exit(1);
    }

    const unsigned int N = g->numVertices;
    const float divDN = DAMPING_FACTOR/N;
    const float minusD = 1 - DAMPING_FACTOR;
    const float divN = 1.0f/(float)N;
    const int actualNJob = N / MAX_THREADS;

    float* lastStep = malloc(sizeof(float) * N);
    if (lastStep == NULL) {
        printf("failed to malloc odd\n");
        exit(1);
    }
    float* currentStep = malloc(sizeof(float) * N);
    if (currentStep == NULL) {
        printf("failed to malloc even\n");
        exit(1);
    }

    // each rank start with the value 1/N
    for(int i = 0; i < N; i++) {
        lastStep[i] = divN;
    }

    // calculating all the vertices index that their out degree is 0
    struct calculatePageWithOutLinkArgs* pagesWithoutLink = malloc(MAX_THREADS * sizeof(struct calculatePageWithOutLinkArgs));
    if (pagesWithoutLink == NULL) {
        printf("failed to malloc calculatePageWithOutLinkArgs\n");
        exit(1);
    }
    for (int i = 0; i < MAX_THREADS; i++) {
        pagesWithoutLink[i].start = i * actualNJob;
        pagesWithoutLink[i].adjacencyNumber = g->adjacencyNumber;
        if(i == MAX_THREADS - 1) {
            pagesWithoutLink[i].end = N;
        } else {
            pagesWithoutLink[i].end = pagesWithoutLink[i].start + actualNJob;
        }
        pagesWithoutLink[i].result = NULL;
        thr_pool_queue(thr, calculatePageWithOutLink, &pagesWithoutLink[i]);
    }
    thr_pool_wait(thr);

    struct rankCalculationArgsV2* argsArrayRank = malloc(MAX_THREADS * sizeof(struct rankCalculationArgsV2));
    if (argsArrayRank == NULL) {
        printf("failed to malloc rankCalculationArgsV2\n");
        exit(1);
    }
    int flag = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < MAX_THREADS; j++) {
            argsArrayRank[j].N = N;
            argsArrayRank[j].dn = divDN;
            argsArrayRank[j].minusD = minusD;
            argsArrayRank[j].start = j*actualNJob;
            if(j == MAX_THREADS - 1) {
                // if last thread get all the leftover jobs
                argsArrayRank[j].end = N;
            } else {
                argsArrayRank[j].end = argsArrayRank[j].start + actualNJob;
            }
            argsArrayRank[j].pointerToMe = g->pointerToMe;
            argsArrayRank[j].adjacencyNumber = g->adjacencyNumber;
            argsArrayRank[j].rankUJDivN = 0;
            argsArrayRank[j].PageWithoutLinkArgs = pagesWithoutLink;
            argsArrayRank[j].PageWithoutLinkArgsSize = MAX_THREADS;
            argsArrayRank[j].rankBefore = lastStep;
            argsArrayRank[j].res = currentStep;
            thr_pool_queue(thr, rankCalculationV2, &argsArrayRank[j]);
        }
        thr_pool_wait(thr);
        float *temp = lastStep;
        lastStep = currentStep;
        currentStep = temp;
        flag = 1 - flag;
    }
    memcpy(rank, lastStep, g->numVertices * sizeof(float));
    for(int i = 0; i < MAX_THREADS; i++) {
        // freeing calculatePageWithOutLinkArgs
        PageWithoutLink * current = pagesWithoutLink[i].result;
        PageWithoutLink * prev = NULL;
        while(current != NULL) {
            prev = current->next;
            free(current);
            current = prev;
        }
    }
    free(thr);
    free(argsArrayRank);
    free(pagesWithoutLink);
    free(lastStep);
    free(currentStep);
}
