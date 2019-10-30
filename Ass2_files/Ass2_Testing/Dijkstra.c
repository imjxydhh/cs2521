#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

static ShortestPaths newPath(Vertex src, int size);
static PredNode *newPredNode(Vertex pre);
static void insertPredNode(PredNode **pred, Vertex src, PredNode *node);
static void releasePred(PredNode *pred);


ShortestPaths dijkstra(Graph g, Vertex src) {
	int size = GraphNumVertices(g);
	ShortestPaths result = newPath(src, size);
	
	PQ pq = PQNew();

	PQAdd(pq, (ItemPQ){src, 0});
	while(!PQIsEmpty(pq)){
	// Priority queue is empty means that all reachable vertex has been visited
		ItemPQ min = PQDequeue(pq);
		AdjList curr =  GraphOutIncident(g, min.key);
		while(curr != NULL){
		// Go through all incident edges of the current minimum node
			PredNode *newPre = newPredNode(min.key);
			if(result.dist[curr->v] == 0 && curr->v != src){
			// The case that the node that this edge goes to has not been visited 
				result.dist[curr->v] = min.value + curr->weight;
				insertPredNode(result.pred, curr->v, newPre);
				PQAdd(pq, (ItemPQ){curr->v, result.dist[curr->v]});
			}else if(result.dist[curr->v] == min.value + curr->weight){
			// The case that there are more than one shortest path to the node that this edge goes to
				insertPredNode(result.pred, curr->v, newPre);
			}else if(result.dist[curr->v] > min.value + curr->weight){
			// The case that a shorter path to the node that this edge goes to is found
				result.dist[curr->v] = min.value + curr->weight;
				
				// Release all the precede nodes as a shorter path is found
				releasePred(result.pred[curr->v]);
				result.pred[curr->v] = NULL;
				insertPredNode(result.pred, curr->v, newPre);

				PQUpdate(pq, (ItemPQ){curr->v, result.dist[curr->v]});
			}else{
				free(newPre);
			}
			curr = curr->next;
		}
	}
	PQFree(pq);
	return result;
}

void showShortestPaths(ShortestPaths sps) {
	printf("number of nodes: %d\n", sps.numNodes);
	printf("src: %d\n", sps.src);
	printf("distances:\n");
	for(int i = 0;i < sps.numNodes;i++){
		printf("  %d: %d\n", i, sps.dist[i]);
	}
	printf("preds:\n");
	for(int i = 0;i < sps.numNodes;i++){
		printf("  %d: ", i);
		PredNode *curr = sps.pred[i];
		while(curr != NULL){
			printf("%d -> ", curr->v);
			curr = curr->next;
		}
		printf("NULL\n");
	}
}

/**
 * It only frees those memory that is allocated on heap such as
 * the distance array and the precede nodes lists array 
 */
void freeShortestPaths(ShortestPaths sps) {
	free(sps.dist);
	for(int i = 0;i < sps.numNodes;i++){
		releasePred(sps.pred[i]);
	}
	free(sps.pred);
}

/**
 * This function is used to create and initialize
 * a new ShortestPath struct. The new ShortestPath is 
 * not entirely on stack. The distance array and the 
 * precede nodes lists array are allocated on heap.  
 */
static ShortestPaths newPath(Vertex src, int size){
	ShortestPaths path;
	path.numNodes = size;
	path.dist = malloc(size * sizeof(int));
	if(path.dist == NULL){
		fprintf(stderr, "Memory allocation failed.\n");
		abort();
	}

	// Initialization
	for(int i = 0;i < size;i++){
		path.dist[i] = 0;
	}
	path.dist[src] = 0;
	path.src = src;

	path.pred = calloc(size, sizeof(PredNode*));
	if(path.pred == NULL){
		fprintf(stderr, "Memory allocation failed.\n");
		abort();
	}
	return path;
}

/**
 * This function is used to create and initialize
 * a new PredNode. The new node is allocated on heap. 
 */
static PredNode *newPredNode(Vertex pre){
	PredNode *newNode = malloc(sizeof(*newNode));
	newNode->next = NULL;
	newNode->v = pre;
	return newNode;
}

/**
 * Insert a new PredNode into a existed PredNode list. 
 */
static void insertPredNode(PredNode **pred, Vertex target, PredNode *node){
	if(pred[target] == NULL){
		pred[target] = node;
	}else{
		PredNode *next = pred[target]->next;
		pred[target]->next = node;
		node->next = next;
	}
}

/**
 * Release a PredNode list. 
 */
static void releasePred(PredNode *pred){
	if(pred != NULL){
		releasePred(pred->next);
		free(pred);
	}
}