// Interface for a priority queue ADT
// This priority queue stores items of type ItemPQ, and uses the 'value'
// field as the priority, with smaller values having higher priority.
// COMP2521 Assignment 2

// Note: You MUST NOT modify this file.

#include <stdbool.h>

#ifndef PQ_H
#define PQ_H

typedef struct PQRep *PQ;

typedef struct ItemPQ {
	int key;
	int value;
} ItemPQ;

/**
 * Creates a new priority queue
 */
PQ PQNew(void);

/**
 * Adds  an  item to the priority queue. If an item with the given 'key'
 * field  exists,  its 'value' field is updated to match the given item.
 * The update is treated as if the item was removed and then reinserted.
 */
void PQAdd(PQ pq, ItemPQ item);

/**
 * Removes and returns the item with the smallest 'value' field from the
 * priority  queue.  If  multiple items have the same 'value' field, the
 * item which was inserted the earliest out of those will be removed.
 */
ItemPQ PQDequeue(PQ pq);

/**
 * Updates  the 'value' field of the item in the priority queue with the
 * given 'key' field to the given 'value' field. If  there  is  no  item
 * with the given 'key' field in the priority queue, no action is taken.
 * The update is treated as if the item was removed and then reinserted.
 */
void PQUpdate(PQ pq, ItemPQ item);

/**
 * Returns true if the given priority queue is empty, or false otherwise
 */
bool PQIsEmpty(PQ pq);

/**
 * Prints the given priority queue to stdout
 */
void PQShow(PQ pq);

/**
 * Frees all memory associated with the given priority queue
 */
void PQFree(PQ pq);

#endif

