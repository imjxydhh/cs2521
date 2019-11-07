// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"

/**
 * Core function that find the total number of paths and total number of paths that 
 * go through a specified vertex
 * @param paths : The array that store the shortest paths from every node
 * @param pathNum : The two-dimension array used to store the total number of paths
 * between any pair of vertices
 * @param pathNumV : A three-dimension array used to store the total number of paths
 * between any pari of vertices that pass through any vertex
 */
static void countPath(ShortestPaths *paths, int **pathNum, int ***pathNumV, 
						Vertex src, Vertex dest, Vertex via);

NodeValues closenessCentrality(Graph g) {
	int nums = GraphNumVertices(g);
	NodeValues nvs = {nums, calloc(nums, sizeof(double))};
	
	// Two layer of loop to find every vertex's closeness centrality
	for(Vertex i = 0;i < nums;i++){
		ShortestPaths path = dijkstra(g, i);
		
		// The number of vertices that the current vertex can reach
		int numReach = 0;

		int sum = 0;
		for(Vertex j = 0;j < nums;j++){
			if(path.dist[j] != 0){
			// Judge if there is a path(distance > 0 then there is a path)  
				numReach++;
				sum += path.dist[j];
			}
		}
		if(sum){
		// If sum is 0(which is false) then it can not reach any vertices so the result is 0(initial value)
			nvs.values[i] = (double)(numReach) * (numReach) / ((nums-1) * sum);
		}
		freeShortestPaths(path);  
	}
	return nvs;
}

NodeValues betweennessCentrality(Graph g) {
	int nums = GraphNumVertices(g);
	NodeValues nvs = {nums, calloc(nums, sizeof(double))};

	// This is an array of pointer to int 
	int *pathNum[nums];
	
	// This is an array of pointer to pointer of int
	int **pathNumV[nums];
	
	for (Vertex i = 0; i < nums; i++){
		
		// Allocation memory to the array of pointer to int to form a 2-dimension array
		pathNum[i] = calloc(nums, sizeof(int));
		
		// Allocation memory to the the array to form a 2-dimension pointer array
		pathNumV[i] = calloc(nums, sizeof(int*));
	}

	for (Vertex i = 0; i < nums; i++){
		for (Vertex j = 0; j < nums; j++){
			
			// Allocation memory two the 2-dimension pointer array to form a 3-dimension array
			pathNumV[i][j] = malloc(nums * sizeof(int));

			for(int k = 0;k < nums;k++){
			// Initialization
				pathNumV[i][j][k] = -1;
			}
		}
	}

	// Find shortest path of every source vertex
	ShortestPaths paths[nums];
	for (Vertex i = 0; i < nums; i++){
		paths[i] = dijkstra(g, i);
	}

	// It's a three layer loop to ensure that each vertex is pass through once
	for (Vertex i = 0; i < nums; i++){

		// This is the betweenness closeness value we are finding
		double sum = 0;

		for(Vertex j = 0;j < nums;j++){
			for(Vertex k = 0;k < nums;k++){	
				if(pathNumV[j][k][i] == -1 && j != k){
					countPath(paths, pathNum, pathNumV, j, k, i);
				}else if(j == k){
					continue;
				}
				if(pathNum[j][k]){
					sum += ((double)pathNumV[j][k][i] / pathNum[j][k]);
					// printf("Vertex: %d;sigma(%d,%d)(v): %d;sigma(%d,%d): %d\n", i,j,k, pathNumV[j][k][i],j,k, pathNum[j][k]);
				}
			}
		}
		nvs.values[i] = sum;
	}
	// printf("%d\n", pathNum[47][46]);getchar();
	/* for (Vertex i = 0; i < nums; i++){
		for(Vertex j = 0;j < nums;j++){
			printf("%d ", pathNum[i][j]);
		}
		printf("\n");
	}  */

	// Free all allocated memory
	for(Vertex i = 0;i < nums;i++){
		freeShortestPaths(paths[i]);
	}
	for (Vertex i = 0; i < nums; i++){
		free(pathNum[i]);
		for(Vertex j = 0;j < nums;j++){
			free(pathNumV[i][j]);
		}
		free(pathNumV[i]);
	}

	return nvs;
}
/**
 * This algorithm stores all visited paths' result into an array, so each possible 
 * combination of source vertex, destination vertex and the vertex passed through 
 * will be visited at most once, so the time complexity is O(cube of V) where V is 
 * the number of vertices 
 */

/**
 * This function is just a variant of the above funciton
 * It simply does some calculation to the result of the above function 
 */
NodeValues betweennessCentralityNormalised(Graph g) {
	NodeValues nvs = betweennessCentrality(g);
	for(Vertex i = 0;i < nvs.numNodes;i++){
		nvs.values[i] /=  ((nvs.numNodes-1) * (nvs.numNodes-2));
	}
	return nvs;
}

void showNodeValues(NodeValues nvs) {
	for(int i = 0;i < nvs.numNodes;i++){
		printf("%d: %lf\n", i, nvs.values[i]);
	}
}

void freeNodeValues(NodeValues nvs) {
	free(nvs.values);
}

/**
 * As the number of shortest paths(no matter if we need个it the path to pass through some vertex) 
 * from one vertex to another vertex, we only need to konw the number of paths from src to the 
 * precede nodes in all possible shortest path. So we could write this function in a recursive way 
 * @param paths : The shortestPath array 
 * @param pathNum : The 2-dimension array used to store the total number of shortest paths
 * @param pathNumV : The 3-dimension array used to store the number of shortest paths that pass 
 * through a specified vertex 
 */
static void countPath(ShortestPaths *paths, int **pathNum, int ***pathNumV, 
						Vertex src, Vertex dest, Vertex via){
	
	// Get the precede vertices array from src to dest
	PredNode *curr = paths[src].pred[dest];

	// Number of shortest paths that start from src to dest and pass through
	int numVia = 0;

	// Total number of shortest paths starting from src to dest
	int num = 0;

	while(curr != NULL){
	// Go through all shortest path(every precede vertices)
		if(curr->v == src){
		// Base case src can directly reach dest
			num ++;
		}else{
			if(pathNumV[src][curr->v][via] == -1){
			// The path from src to the precede node has not been visited yet
				countPath(paths, pathNum, pathNumV, src, curr->v, via);
			}

			// Accumulate the number of paths for each precede vertices()
			num += pathNum[src][curr->v];

			if(curr->v == via){
			// The case that the precede vertex is the vertex to be passed through
				/**
				 * In this case, the total number of shortest path from src to the vertex to
				 * be passed through are all paths that start from src to the current vertex and
				 * passed through via 
				 */
				numVia += pathNum[src][curr->v];
			}else{
			// Opposite
				numVia += pathNumV[src][curr->v][via];
			}

		}
		curr = curr->next;
	}
    pathNumV[src][dest][via] = numVia;
	pathNum[src][dest] = num;
}