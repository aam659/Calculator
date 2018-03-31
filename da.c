/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 0 */
/* Date: 7 September, 2017 */

/*This is the implementation file for the header file "da.h",
which is used to create a dynamic array 'class' for generic data types.*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "da.h"

struct da {
	int capacity;						// DA capacity
	int size;						// DA size
	void (*display)(FILE *, void *);			// display field
	void **DynamicArray;					// DA array
};

/*************** private interface **************/

/* grow method */
/* This method doubles the array capacity. */

static void grow(DA *items, int *cap) {
	int i;
	*cap *= 2;						// Increases capacity-pass by ref.

	DA *temp = malloc(sizeof(DA));				// Create dummy array for copying
	assert(temp != 0);

	temp->capacity = (*cap);
	temp->size = items->size;
	temp->display = items->display;

	temp->DynamicArray = malloc(sizeof(void *) * (*cap));	// Allocate space for dummy array
    	assert((temp->DynamicArray) != 0);

	for(i = 0; i < items->size; ++i) {
		temp->DynamicArray[i] = items->DynamicArray[i];	// Copy array values
	}

	free(items->DynamicArray);				// Release space from old DA array

	items->DynamicArray = temp->DynamicArray;

	return;
}

/* shrink method */
/* This method halves the array capacity when 
the ratio of size to capacity is less than 25% */

static void shrink(DA *items, int *cap) {
	int i;
	*cap /= 2;						// Decreases capacity-pass by ref.

	assert(*cap >= 1);					// Ensure that capacity is greater than 1

	DA *temp = malloc(sizeof(DA));				// Create dummy array for copying
	assert(temp != 0);

	temp->capacity = (*cap);
	temp->size = items->size;
	temp->display = items->display;

	temp->DynamicArray = malloc(sizeof(void *) * (*cap));	// Allocate space for dummy array
    	assert((temp->DynamicArray) != 0);

	for(i = 0; i < items->size; ++i) {
		temp->DynamicArray[i] = items->DynamicArray[i];	// Copy array values
	}

	free(items->DynamicArray);				// Release space from old DA array

	items->DynamicArray = temp->DynamicArray;

	return;
}

/*************** public interface ***************/

/* constructor */
/* This is the constructor that initializes
the fields for the given struct da. */

DA *newDA(void (*d)(FILE *, void *)) {				// d is the display function
	DA *items = malloc(sizeof(DA));				// Allocates space for DA object
	assert(items != 0);

	items->capacity = 1;					// Initialize capacity and size
	items->size = 0;
	items->display = d;					// Initialize display field to passed display method

	items->DynamicArray = malloc(sizeof(void *) * (items->capacity));
	assert(items->DynamicArray != 0);

	return items;
}

/* accessor */
/* This is the accessor for the class. */

void *getDA(DA *items, int index) {
	assert((index >= 0) && (index < (items->size)));
	
    	return items->DynamicArray[index];			// Returns specified index
}

/* mutator */
/* This is the mutator for the class. */

void *setDA(DA *items, int index, void *value) {
	assert((index >= 0) && (index <= (items->size)));

	if (index == (items->size)) {				// Setting a value in an empty space returns NULL
        	insertDA(items, value);
        	return NULL;
    	}

    
        void *oldValue = items->DynamicArray[index];
        items->DynamicArray[index] = value;			// Setting a value in a used space returns entry
	return oldValue;
}

/* array insertion */
/* This is the array insertion method. 
This method calls the grow function accordingly. */

void insertDA(DA *items, void *value) {
	if ((items->size) == (items->capacity)) {		// Dynamic array resizing if size == capacity
		grow(items, &(items->capacity));
	}

	items->DynamicArray[(items->size)] = value;		// Insert at back
	items->size += 1;					// Increase size

	return;
}

/* array deletion */
/* This is the array deletion method. 
This method calls the shrink function accordingly. */

void *removeDA(DA *items) {
	assert(items->size != 0);
	// Stores removed value
	void *removedValue = items->DynamicArray[items->size - 1];
	
	(items->size) -= 1;					// Decrease size

	// Dynamic array resizing if ratio of size to capacity is < 25%
	if ((((double)(items->size)) / (items->capacity)) < .25 && (items->capacity != 1)) {	
		shrink(items, &(items->capacity));
	}

	assert((items->size) >= 0);					

	return removedValue;					// Returns removed value
}

/* array union */
/* This method unions two arrays,
leaving the donor array empty upon return. */

void unionDA(DA *recipient, DA *donor) {
	int i;
	int j= 0;
	int donSize = (donor->size);
	int recipSize = (recipient->size);
	int combinedSize = recipSize + donSize;
	void **array = extractDA(donor);

	if (combinedSize > recipient->capacity) {
		while (combinedSize > recipient->capacity) {
			recipient->capacity *= 2;		// Increases capacity-pass by ref.
		}

		DA *temp = malloc(sizeof(DA));			// Create dummy array for copying
		assert(temp != 0);

		temp->capacity = recipient->capacity;
		temp->size = recipient->size;
		temp->display = recipient->display;

		// Allocate space for dummy array
		temp->DynamicArray = malloc(sizeof(void *) * (recipient->capacity));	
    		assert((temp->DynamicArray) != 0);

		for (i = 0; i < combinedSize; ++i) {
			if ((i >= 0) && (i < recipSize)) {
				temp->DynamicArray[i] = recipient->DynamicArray[i];
			}

			else {
				temp->DynamicArray[i] = array[j];
				temp->size += 1;
				++j;
			}
		}

		free(recipient->DynamicArray);			// Release space from old DA array

		recipient->size = temp->size;
		recipient->DynamicArray = temp->DynamicArray;

		return;
	}

	for (i = 0; i < donSize; ++i) {
		insertDA(recipient, array[i]);
	}

	return;
}

/* C array extraction */
/* This method returns a given array after
setting the current array to a capacity of 1, a
size of 0, and clearing its array entries. */

void **extractDA(DA *items) {
	if (items->size == 0) {
		items->capacity = 1;				// Sets capacity to 1
		return 0;					// Check for extracting an empty array
	}

	if (((items->capacity) - (items->size) > 0) && (items->capacity != 1)) {
        	items->capacity = items->size;			// Shrink-to-fit if capacity != size
    	}

	void **extractedValues = items->DynamicArray;		// Stores extracted values

	DA *newItems = malloc(sizeof(DA));			// Creates dummy array
	assert(newItems != 0);

	newItems->capacity = 1;					// New array capacity and size are 1 and 0, respectively
	newItems->size = 0;
	newItems->display = items->display;			// Initialize display field for dummy array

	newItems->DynamicArray = malloc(sizeof(void *) * (newItems->capacity));
	assert(newItems->DynamicArray != 0);
	
	items->capacity = newItems->capacity;			// Set old array to new array after freeing old array
	items->size = newItems->size;

	items->DynamicArray = newItems->DynamicArray;

    	return extractedValues;
}

/* array size */
/* This method gives the array size. */

int sizeDA(DA *items) {
	return items->size;					// Returns array size
}

/* visualize method */
/* This method prints the DA, along
with an indication of how much space is 
remaining. */

void visualizeDA(FILE *fp, DA *items) {
	int i = 0;

	fprintf(fp, "[");

	if (items->size != 0) {
		for (i = 0; i < items->size; ++i) {		// Displays array using display field for DA
        		items->display(fp, items->DynamicArray[i]);
			if ((items->size > 1) && (i != items->size -1)) {
				fprintf(fp, ",");
			}
        	}
	}

	fprintf(fp, "][%d]", ((items->capacity) - (items->size)));

	return;
}

/* display method */
/* This method displays the given array
using the given display method. */

void displayDA(FILE *fp, DA *items) {
    	int i;

    	fprintf(fp, "[");

	if (items->size != 0) {					// Displays array using display field for DA 
        	for (i = 0; i < items->size; ++i) {
            		items->display(fp, items->DynamicArray[i]);
			if ((items->size > 1) && (i != items->size -1)) {
				fprintf(fp, ",");
			}
            	}
	}

	fprintf(fp, "]");
	return;
}
