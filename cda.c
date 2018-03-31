/*Author: Alan Manning*/
/*Class: CS201 Data Structures and Algorithms*/
/*Assignment 0*/
/*7 September, 2017*/

/*This is the implementation file for the header file "cda.h",
which is used to create a circular dynamic array 'class' for generic data types.*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "cda.h"

struct cda {
	int capacity;						// CDA capacity
	int size;						// CDA size
	int startIndex;						// CDA startIndex
	int endIndex;						// CDA endIndex
	void (*display)(FILE *, void *);			// CDA display method
	void **CircDynamArray;					// CDA array
};

/*************** private interface ***************/

/* increment index method */
/* This method increments the index of the CDA. */

static int incrementIndex(CDA *items, int index, int increment) {
	int modIndex = index + increment;			// Corrects index

	if ((modIndex) >= (items->capacity)) {
		modIndex -= (items->capacity);
	}

	return modIndex;
}

/* decrement index method */
/* This method decrements the index of the CDA. */

static int decrementIndex(CDA *items, int index, int decrement) {
	int modIndex = index - decrement;			// Corrects index

	if (modIndex < 0) {
		modIndex += (items->capacity);
	}

	return modIndex;
}

/* growFront method */
/* This method doubles the CDA when using insertCDAfront. */

static void growFront(CDA *items, int *cap, void *value) {
	int i;

	CDA *temp = malloc(sizeof(CDA));			// Creates dummy array for copying old array values
	assert(temp != 0);

	items->size += 1;					// Increases array size

	temp->capacity = (*cap);
	temp->size = items->size;
	temp->display = items->display;
	temp->startIndex = 0;
	temp->endIndex = items->size;

	temp->CircDynamArray = malloc(sizeof(void *) * (*cap * 2));
	assert((temp->CircDynamArray) != 0);

	temp->CircDynamArray[0] = value;			// Insert at front of CDA

	for (i = 1; i < temp->size; ++i) {
		temp->CircDynamArray[i] = getCDA(items, i - 1);	// Copies array values
	}

	free(items->CircDynamArray);				// Frees old CDA array

	items->CircDynamArray = temp->CircDynamArray;		// Sets CDA array to dummy array
	items->startIndex = 0;
	items->endIndex = items->size;
	*cap *= 2;						// Doubles capacity

	return;
}

/* growBack method */
/* This method doubles the CDA size when insertCDAback is called. */

static void growBack(CDA *items, int *cap, void *value) {
	int i;

	CDA *temp = malloc(sizeof(CDA));			// Creates dummy array to copy array values		
	assert(temp != 0);

	temp->capacity = (*cap);
	temp->size = items->size;
	temp->display = items->display;
	temp->startIndex = 0;

	temp->CircDynamArray = malloc(sizeof(void *) * (*cap * 2));
	assert((temp->CircDynamArray) != 0);

	for (i = 0; i < temp->size; ++i) {			// Insert at back and copy array values
		temp->CircDynamArray[i] = getCDA(items, i);
	}

	free(items->CircDynamArray);				// Frees old CDA array

	temp->size += 1;					// Increase array size
	items->size += 1;
	temp->endIndex = temp->size;
	temp->CircDynamArray[temp->endIndex - 1] = value;
	items->CircDynamArray = temp->CircDynamArray;
	items->startIndex = 0;
	items->endIndex = items->size;

	*cap *= 2;						// Doubles capacity

	return;
}

/* shrink method */
/* This method shrinks array capacity by half when remove methods are called. */

static void shrink(CDA *items, int *cap) {
	int i;
	assert(*cap >= 1);

	CDA *temp = malloc(sizeof(CDA));			// Creates dummy array
	assert(temp != 0);

	temp->capacity = (*cap);
	temp->size = items->size;
	temp->display = items->display;
	temp->startIndex = 0;
	temp->endIndex = temp->size;

	temp->CircDynamArray = malloc(sizeof(void *) * (*cap / 2));
	assert((temp->CircDynamArray) != 0);

	for(i = 0; i < temp->size; ++i) {			// Copies array values
		temp->CircDynamArray[i] = getCDA(items, i);
	}

	free(items->CircDynamArray);				// Frees old CDA array

	items->CircDynamArray = temp->CircDynamArray;		// Sets CDA array to dummy array values
	items->startIndex = 0;

	items->endIndex = items->size;

	*cap /= 2;						// Reduces capacity by half

	return;
}

/*************** public interface ***************/

/* constructor */
/* This is the constructor for the CDA objects. */

CDA *newCDA(void (*d)(FILE *,void *)) {
	CDA *items = malloc(sizeof(CDA));			// Allocate space for CDA object and initialize fields
	assert(items != 0);

	items->capacity = 1;
	items->size = 0;
	items->display = d;					// Initialize display method to passed display method
	items->startIndex = items->endIndex = 0;		// Start and end indices are 0
	
	items->CircDynamArray = malloc(sizeof(void *) * (items->capacity));
	assert(items->CircDynamArray != 0);

	return items;
}

/* insert front method */
/* This is the insertCDAfront method */

void insertCDAfront(CDA *items,void *value) {
	if (items->size == 0) {					// If size = 0, calls insertCDAback method
		insertCDAback(items, value);
		return;
	}

	if ((items->size) == (items->capacity)) {		// If size == capacity, calls grow method to double
		growFront(items, &(items->capacity), value);
		return;
	}

	// Updates start index
	items->startIndex = decrementIndex(items, items->startIndex, 1);
	items->CircDynamArray[(items->startIndex)] = value;	// Insert at front
	items->size += 1;					// Increase size

	return;
}

/* insert back method */
/* This is the insertCDAback method. */

void insertCDAback(CDA *items,void *value) {
	if ((items->size) == (items->capacity)) {		// If size == capacity, calls grow method to double
		growBack(items, &(items->capacity), value);
		return;
	}

	items->CircDynamArray[(items->endIndex)] = value;	// Insert at back

	// Update end index
	items->endIndex = incrementIndex(items, items->endIndex, 1);

	items->size += 1;					// Increases size

	return;
}

/* remove front method */
/* Removes from front of array and updates start index. */

void *removeCDAfront(CDA *items) {
	assert(items->size != 0);				// Stores removed value
	void *frontRemoved = items->CircDynamArray[items->startIndex];	
	
	if (items->size > 1) {					// If size > 1, update start index
		items->startIndex = incrementIndex(items, items->startIndex, 1);
	}
	
	items->size -= 1;					// Decrease size by 1

	if ((((double)(items->size)) / (items->capacity)) < .25) {
		shrink(items, &(items->capacity));		// Shrinks size if given ratio is below 25%
	}
	
	assert((items->size) >= 0);

	return frontRemoved;					// Returns removed value
}

/* remove back method */
/* Removes from back of array and updates end index. */

void *removeCDAback(CDA *items) {
	assert(items->size != 0);				// Stores removed value
	void *backRemoved = items->CircDynamArray[items->endIndex - 1];

	if (items->size > 1) {					// If size > 1, update end index
		items->endIndex = decrementIndex(items, items->endIndex, 1);
	}
	
	items->size -= 1;					// Decreases size by 1

	if ((((double)(items->size)) / (items->capacity)) < .25) {
		shrink(items, &(items->capacity));
	}

	assert((items->size) >= 0);

	return backRemoved;
}

/* union method */
/* This method creates a union between a recipient CDA
and a donor CDA, increasing the recipient capacity
accordingly and removing all entries from the donor CDA. */

void unionCDA(CDA *recipient,CDA *donor) {
	int i;
	int donSize = (donor->size);				// Stores donor size
	void **array = extractCDA(donor);			// Extracts and stores donor array

	for (i = 0; i < donSize; ++i) {
		insertCDAback(recipient, array[i]);		// Inserts donor entries
	}

	return;
}

/* accessor */
/* This is the accessor for the CDA class. */

void *getCDA(CDA *items,int index) {
	assert((index >= 0) && (index < (items->size)));	// Index must be in the range [0, size - 1]

	// Adjusts index based on the user's perspective
	int actualIndex = incrementIndex(items, items->startIndex, index);

	return items->CircDynamArray[actualIndex];
}

/* mutator */
/* This is the mutator for the CDA class. */

void *setCDA(CDA *items,int index,void *value) {
	assert((index >= 0) && (index <= (items->size)));	// Index must be in the range [0, size - 1]

	if (index == (items->size)) {
		insertCDAback(items, value);			// Calls insertCDAfront to insert item at front
		return NULL;					// Returns NULL when no entry to replace
	}

	void *oldValue = getCDA(items, index);			// Stores old value to be returned and replaces it in array
	int actualIndex = incrementIndex(items, items->startIndex, index);
	items->CircDynamArray[actualIndex] = value;
	return oldValue;
}

/* C array extraction */
/* This method extracts the underlying C
array, removes the original array values, and
sets the array size to 0 and capacity to 1. */

void **extractCDA(CDA *items) {
	int i;
	if (items->size == 0) {
		items->capacity = 1;				// Check for extracting from an empty array
		return 0;					// Returns NULL pointer
	}

	void **extractedValues = malloc(sizeof(extractedValues) * (items->size));

	for (i = 0; i < items->size; ++i) {
		*(extractedValues + i) = getCDA(items, i);	// Stores underlying array to be returned
	}
	
	if (((items->capacity) - (items->size) > 0) && (items->capacity != 1)) {
		items->capacity = items->size;			// Shrink-to-fit if size != capacity
	}


	CDA *newItems = malloc(sizeof(CDA));			// Creates dummy CDA object to reset current CDA array
	assert(newItems != 0);

	newItems->capacity = 1;
	newItems->size = 0;
	newItems->display = items->display;
	newItems->startIndex = newItems->endIndex = 0;

	newItems->CircDynamArray = malloc(sizeof(void *) * (newItems->capacity));
	assert(newItems->CircDynamArray != 0);

	items->capacity = newItems->capacity;
	items->size = newItems->size;
	items->CircDynamArray = newItems->CircDynamArray;	// Stores new array in old array
	items->startIndex = items->endIndex = 0;

	return extractedValues;
}

/* array size */
/* This method returns the size of the array. */

int sizeCDA(CDA *items) {
	return items->size;
}

/* visualize method */
/* This method displays the CDA array using the
passed display method, along with an indicator for
the amount of space remaining. */

void visualizeCDA(FILE *fp,CDA *items) {
	int i;

	fprintf(fp, "(");
	
	if (items->size != 0) {
		for (i = 0; i < items->size; ++i) {
			void *index = getCDA(items, i);
			items->display(fp, index);		// Calls display method to display CDA values
			if ((items->size > 1) && (i != items->size -1)) {
				fprintf(fp, ",");
			}
		}
	}
	
	fprintf(fp, ")(%d)", ((items->capacity) - (items->size)));

	return;
}

/* display method */
/* This method displays the CDA array using the
passed display method. */

void displayCDA(FILE *fp,CDA *items) {
	int i;

	fprintf(fp, "(");
	
	if (items->size != 0) {
		for (i = 0; i < items->size; ++i) {
			void *index = getCDA(items, i);
			items->display(fp, index);		// Calls display method to display CDA values
			if ((items->size > 1) && (i != items->size -1)) {
				fprintf(fp, ",");
			}
		}
	}
	
	fprintf(fp, ")");
	return;
}



