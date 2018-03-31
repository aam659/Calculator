/* Author: Alan Manning */
/* Course: CS 201 Data Structures and Algorithms */
/* Assignment 0 */
/* Date: 07 September, 2017 */

/* This is the header file for a dynamic array class.
The class includes the following methods and fields and 
uses an object oriented approach to implement an array that
contains generic data values as its entries. */

#ifndef __DA_INCLUDED__
#define __DA_INCLUDED__

#include <stdio.h>

typedef struct da DA;

extern DA *newDA(void (*d)(FILE *,void *)); 
extern void insertDA(DA *items,void *value);
extern void *removeDA(DA *items);
extern void unionDA(DA *recipient,DA *donor);
extern void *getDA(DA *items,int index);
extern void *setDA(DA *items,int index,void *value);
extern void **extractDA(DA *items);
extern int sizeDA(DA *items);
extern int capacityDA(DA *items);
extern void visualizeDA(FILE *fp,DA *items);
extern void displayDA(FILE *fp,DA *items);
#endif

