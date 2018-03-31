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
#include "stack.h"
#include "da.h"

struct stack {
	void (*display)(FILE *, void *);			// display field
	DA *stackItem;						// DA array item for stack
	int size;						// Size of stack
};

/*************** public interface ***************/

/* constructor */
/* This is the constructor for the stack objects. */

STACK *newSTACK(void (*d)(FILE *,void *)) {
	STACK *items = malloc(sizeof(STACK));			// Allocate space for stack object	
	assert(items != 0);

	items->stackItem = newDA(d);				// Initialize DA array for stack
	items->size = 0;					// Set stack size to 0
	items->display = d;					// Set display field to passed display method

	return items;
}

/* push method */
/* This is the method that pushes items onto the stack. */

void push(STACK *items,void *value) {
	insertDA(items->stackItem, value);			// Calls insertDA method to push onto stack
	sizeSTACK(items);					// Calls size method to correct stack size
	return;
}

/* pop method */
/* This is the pop method that pops items from the stack
and returns them. */

void *pop(STACK *items) {
	assert(items->size > 0);
	void *poppedValue = removeDA(items->stackItem);		// Calls removeDA method to pop items from stack
	sizeSTACK(items);					// Calls size method to correct stack size
	return poppedValue;					// Returns popped value
}

/* peek method */
/* This is the peek method that shows the top item on the stack. */

void *peekSTACK(STACK *items) {	
	assert(items->size > 0);
	void *top = getDA(items->stackItem, items->size - 1);	// Stores top value on stack and returns value
	return top;
}

/* stack size */
/* This is the method that returns the stack size. */

int sizeSTACK(STACK *items) {
	items->size = sizeDA(items->stackItem);			// Calls sizeDA method to get stack size

	return items->size;
}

/* display method */
/* This method displays the values in the stack, 
along with an indication of how much space is left
in the stack. */

void displaySTACK(FILE *fp,STACK *items) {
	int i;

	fprintf(fp, "|");

	if(items->size != 0) {
		for (i = items->size - 1; i >= 0; --i) {	// Displays stack values using user display method
			items->display(fp, getDA(items->stackItem, i));
			if ((items->size > 1) && (i != 0)) {
				fprintf(fp, ",");
			}
		}
	}

	fprintf(fp, "|");
	return;
}

/* visualize method */
/* This method displays the values in the stack
by calling the display method from the parent class. */

void visualizeSTACK(FILE *fp,STACK *items) {
	displayDA(fp, items->stackItem);			// Calls displayDA method to display values in stack
	return;
}

