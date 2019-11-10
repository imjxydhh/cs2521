// Lance-Williams Algorithm for Hierarchical Agglomerative Clustering
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "LanceWilliamsHAC.h"

/**
 *  Create and initialize and return a new Dendrogram node 
 */
static Dendrogram newDNode(Vertex i);

/** 
 * Two function used when update distance which is respectively corresponding
 * to Complete linkage and Single linkage 
 */
static double max(double n1, double n2){
    
    return n1>n2?n1:n2;
}
static double min(double n1, double n2){
    if(n1 == -1 || n2 == -1){
        return n1 == -1?n2:n1;
    }
    return n1<n2?n1:n2;
}

/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    int numV = GraphNumVertices(g);

    // Funtion pointer
    double (*fun)(double, double);
    
    // Choose a function based on the method parameter
    if(method == 1){
        fun = min;
    }else if(method == 2){
        fun = max;
    }else{
        fprintf(stderr, "Wrong choice of method.\n");
    }

    // Create and initialize Dendrogram array
    Dendrogram gram[numV];
    for(int i = 0;i < numV;i++){
        gram[i] = newDNode(i);
    }

    // Counter that count how many clusters left in the Dendrogram array
    int num = numV;

    // Weight array used to initialize the distance array
    int weight[numV][numV];
    for(int i = 0;i < numV;i++){
        // initialize the array line by line
        for(int j = i;j < numV;j++){
            weight[i][j] = weight[j][i] = 0;
        }

        // After this line is initialized, put weights into this line
        AdjList out = GraphOutIncident(g, i);
        while(out != NULL){
        // Go through all out link from i
            weight[i][out->v] = out->weight;
            out = out->next;
        }
    }
    
    // The distance array
    double dist[numV][numV];
    for(int i = 0;i < numV;i++){
        for(int j = i;j < numV;j++){
            if(j == i){
                dist[i][j] = -1;
            }else{
                if(weight[i][j] == 0 && weight[j][i] == 0){
                // The case that there is no direct link between i and j
                    dist[i][j] = dist[j][i] = -1;
                }else{
                    dist[i][j] = dist[j][i] = 1/(max((double)weight[i][j], (double)weight[j][i]));
                }
            }
        }
    }
    
    while(num > 1){
    // Clustering until there is only one Dendrogram

        // Find the closest two cluster
        double min = -1;
        int src = 0;
        int dest = 0;
        for(int i = 0;i < numV;i++){
            for(int j = i + 1;j < numV;j++){
                if(dist[i][j] != -1){
                    if(min == -1 || dist[i][j] < min){
                        src = i;
                        dest = j;
                        min = dist[i][j];
                    }
                }
            }
        }

        // Merge the two found clusters
        Dendrogram newGram = newDNode(0);
        newGram->left = gram[src];
        newGram->right = gram[dest];
        // The new cluster is put in the position of one of the original clusters(The one with smaller index)
        gram[src] = newGram;
        // Set the position of one of the original clusters to NULL to mark that this cluster has been merged in to another cluster
        gram[dest] = NULL;

        // Update distance array
        for(int i = 0;i < numV;i++){
            if(gram[i] != NULL && src != i){
                dist[src][i] = dist[i][src] = fun(dist[src][i], dist[dest][i]);
            }
            // As one cluster is merged into another cluster, all position related to that cluster is set to infinite
            dist[dest][i] = dist[i][dest] = -1;
        }
        
        num--;
    }

    // After the above loop, there must be only one dendrogram in the array unless the gragh is originally empty
    for(int i = 0;i < numV;i++){
        if(gram[i] != NULL){
            // Return the only dendrogram
            return gram[i];
        }
    }

    // If the gragh is originally empty, return NULL
    // Should never reach here
    return NULL;
}

/**
 * Frees all memory associated with the given Dendrogram structure.
 */
void freeDendrogram(Dendrogram d) {
    if(d->left != NULL){
        freeDendrogram(d->left); 
    }
    if(d->right != NULL){
        freeDendrogram(d->right);
    }
    free(d);
}

static Dendrogram newDNode(Vertex i){
	Dendrogram new = malloc(sizeof(*new));
    new->vertex = i;
    new->left = new->right = NULL;
    return new;
}