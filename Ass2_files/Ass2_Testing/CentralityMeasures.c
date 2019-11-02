// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"

static int countPath(ShortestPaths *paths, int **pathNum, int ***pathNumV, 
						Vertex src, Vertex dest, Vertex via, int size);

NodeValues closenessCentrality(Graph g) {
	int nums = GraphNumVertices(g);
	NodeValues nvs = {nums, calloc(nums, sizeof(double))};
	for(Vertex i = 0;i < nums;i++){
		ShortestPaths path = dijkstra(g, i);
		int numReach = 0;
		int sum = 0;
		for(Vertex j = 0;j < nums;j++){
			if(path.dist[j] != 0){
				numReach++;
				sum += path.dist[j];
			}
		}
		if(sum){
			nvs.values[i] = (double)(numReach) * (numReach) / ((nums-1) * sum);
		}
		freeShortestPaths(path);  
	}
	return nvs;
}

NodeValues betweennessCentrality(Graph g) {
	int nums = GraphNumVertices(g);
	NodeValues nvs = {nums, calloc(nums, sizeof(double))};
	int *pathNum[nums];
	int **pathNumV[nums];
	for (Vertex i = 0; i < nums; i++){
		pathNum[i] = calloc(nums, sizeof(int));
		pathNumV[i] = calloc(nums, sizeof(int*));
	}

	for (Vertex i = 0; i < nums; i++){
		for (Vertex j = 0; j < nums; j++){
			pathNumV[i][j] = malloc(nums * sizeof(int));
			for(int k = 0;k < nums;k++){
				pathNumV[i][j][k] = -1;
			}
		}
	}

	ShortestPaths paths[nums];
	for (Vertex i = 0; i < nums; i++){
		paths[i] = dijkstra(g, i);
	}
	for (Vertex i = 0; i < nums; i++){
		double sum = 0;
		for(Vertex j = 0;j < nums;j++){
			for(Vertex k = 0;k < nums;k++){	
				if(pathNumV[j][k][i] == -1 && j != k){
					countPath(paths, pathNum, pathNumV, j, k, i, nums);
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

static int countPath(ShortestPaths *paths, int **pathNum, int ***pathNumV, 
						Vertex src, Vertex dest, Vertex via, int size){
	PredNode *curr = paths[src].pred[dest];
	int numVia = 0;
	int num = 0;
	while(curr != NULL){
		if(curr->v == src){
			num ++;
		}else{
			if(pathNumV[src][curr->v][via] == -1){
				countPath(paths, pathNum, pathNumV, src, curr->v, via, size);
			}
			num += pathNum[src][curr->v];
			if(curr->v == via){
				numVia += pathNum[src][curr->v];
			}else{
				numVia += pathNumV[src][curr->v][via];
			}
		}
		curr = curr->next;
	}
    pathNumV[src][dest][via] = numVia;
	pathNum[src][dest] = num;

	return numVia;
}