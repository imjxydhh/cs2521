// Implementation of a priority queue ADT
// COMP2521 Assignment 2

// Note: This  implementation  is provided to you, you should not modify
//       it.

// Note: This priority queue implementation uses a linear array and does
//       not  arrange  the items in any particular order. It is designed
//       to be simple and is therefore inefficient.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "PQ.h"

#define DEFAULT_SIZE 64

struct PQRep {
	ItemPQ *items;
	int numItems;
	int capacity;
};

PQ PQNew(void) {
	PQ pq = malloc(sizeof(*pq));
	if (pq == NULL) {
		fprintf(stderr, "Couldn't allocate PQ!\n");
		exit(EXIT_FAILURE);
	}
	
	pq->items = malloc(DEFAULT_SIZE * sizeof(ItemPQ));
	if (pq == NULL) {
		fprintf(stderr, "Couldn't allocate PQ!\n");
		exit(EXIT_FAILURE);
	}
	
	pq->numItems = 0;
	pq->capacity = DEFAULT_SIZE;
	return pq;
}

void PQAdd(PQ pq, ItemPQ item) {
	assert(pq != NULL);
	
	// If key is already in the PQ, use updatePQ
	for (int i = 0; i < pq->numItems; i++) {
		if (pq->items[i].key == item.key) {
			PQUpdate(pq, item);
			return;
		}
	}
	
	// If the PQ is full, expand it (i.e., double its capacity)
	if (pq->numItems == pq->capacity) {
		pq->capacity *= 2;
		pq->items = realloc(pq->items, pq->capacity * sizeof(ItemPQ));
		if (pq->items == NULL) {
			fprintf(stderr, "Couldn't expand PQ!\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Add the new item to the end
	pq->items[pq->numItems] = item;
	pq->numItems++;
}

ItemPQ PQDequeue(PQ pq) {
	assert(pq != NULL);
	assert(pq->numItems > 0);
	
	// Find earliest element with smallest value (highest priority)
	int chosenIndex = 0;
	for (int i = 1; i < pq->numItems; i++) {
		if (pq->items[i].value < pq->items[chosenIndex].value) {
			chosenIndex = i;
		}
	}
	
	ItemPQ element = pq->items[chosenIndex];
	// Shuffle down everything after that element
	for (int i = chosenIndex + 1; i < pq->numItems; i++) {
		pq->items[i - 1] = pq->items[i];
	}
	pq->numItems--;
	
	return element;
}

void PQUpdate(PQ pq, ItemPQ element) {
	assert(pq != NULL);
	
	for (int i = 0; i < pq->numItems; i++) {
		if (pq->items[i].key == element.key) {
			pq->items[i].value = element.value;
			
			// Move updated element to the end
			ItemPQ item = pq->items[i];
			for (int j = i; j < pq->numItems - 1; j++) {
				pq->items[j] = pq->items[j + 1];
			}
			pq->items[pq->numItems - 1] = item;
			return;
		}
	}
}

bool PQIsEmpty(PQ pq) {
	assert(pq != NULL);
	
	return (pq->numItems == 0);
}

void PQShow(PQ pq) {
	assert(pq != NULL);
	
	printf("#items = %d\n", pq->numItems);
	printf("Items:");
	for (int i = 0; i < pq->numItems; i++) {
		printf(" (k: %d, v: %d)", pq->items[i].key, pq->items[i].value);
	}
	printf("\n");
}

void PQFree(PQ pq) {
	assert(pq != NULL);
	
	free(pq->items);
	free(pq);
}

