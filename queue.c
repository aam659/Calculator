/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 0 */
/* Date: 7 September, 2017 */

/*This is the implementation file for the header file "stack.h",
which is used to create a stack 'class' for generic data types.*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "queue.h"
#include "cda.h"

struct queue {						
	void (*display)(FILE *, void *);			// display field
	CDA *queueItem;						// CDA queue items
	int size;						// Queue size
};

/*************** public interface ***************/

/* constructor */
/* This is the constructor for the queue. */

QUEUE *newQUEUE(void (*d)(FILE *,void *)) {
	QUEUE *items = malloc(sizeof(QUEUE));			// Allocate space for queue
	assert(items != 0);

	items->queueItem = newCDA(d);				// Set queue items to CDA object
	items->size = 0;					// Set size of queue to 0
	items->display = d;					// Set display method for queue to that given by user

	return items;
}

/* enqueue method */
/* This is the enqueue method for the queue. */

void enqueue(QUEUE *items,void *value) {
	insertCDAback(items->queueItem, value);			// Calls insertCDAback method to enqueue
	sizeQUEUE(items);					// Corrects size for queue
	return;
}

/* dequeue method */
/* This is the deque method for the queue. */

void *dequeue(QUEUE *items) {
	assert(items->size > 0);
	void *dequeuedValue = removeCDAfront(items->queueItem);	// Calls removeCDAfront method
	sizeQUEUE(items);					// Corrects size for queue
	return dequeuedValue;					// Returns dequeued value
}

/* peek method */
/* This method shows the top of the queue. */

void *peekQUEUE(QUEUE *items) {
	assert(items->size > 0);
	void *top = getCDA(items->queueItem, 0);		// Stores top of queue and returns it
	return top;
}

/* queue size */
/* This method returns the size of the queue. */

int sizeQUEUE(QUEUE *items) {
	items->size = sizeCDA(items->queueItem);		// Calls sizeCDA method to update size of queue
	
	return items->size;
}

/* display method */
/* This method displays the items in the queue,
along with an indication of how much space is left
in the queue. */

void displayQUEUE(FILE *fp,QUEUE *items) {
	int i;
	
	fprintf(fp, "<");

	if(items->size != 0) {
		for (i = 0; i < items->size; ++i) {
			void *index = getCDA(items->queueItem, i);
			items->display(fp, index);		// Displays queue items using given display method
			if ((items->size > 1) && (i != items->size - 1)) {
				fprintf(fp, ",");
			}
		}
	}

	fprintf(fp, ">");
	return;
}

/* visualize method */
/* This method displays the items in the queue
by calling the display method from the parent class. */

void visualizeQUEUE(FILE *fp,QUEUE *items) {
	displayCDA(fp, items->queueItem);			// Calls displayCDA method to display queue items
	return;
}

