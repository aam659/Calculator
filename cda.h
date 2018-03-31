/* Author: Alan Manning */
/* Course: CS 201 Data Structures and Algorithms */
/* Assignment 0 */
/* Date: 07 September, 2017 */

/* This is the header file for a circular dynamic array class.
The class includes the following methods and fields and 
uses an object oriented approach to implement an array that
contains generic data values as its entries. */

#ifndef __CDA_INCLUDED__
#define __CDA_INCLUDED__

#include <stdio.h>

typedef struct cda CDA;

extern CDA *newCDA(void (*d)(FILE *,void *)); 
extern void insertCDAfront(CDA *items,void *value);
extern void insertCDAback(CDA *items,void *value);
extern void *removeCDAfront(CDA *items);
extern void *removeCDAback(CDA *items);
extern void unionCDA(CDA *recipient,CDA *donor);
extern void *getCDA(CDA *items,int index);
extern void *setCDA(CDA *items,int index,void *value);
extern void **extractCDA(CDA *items);
extern int sizeCDA(CDA *items);
extern void visualizeCDA(FILE *,CDA *items);
extern void displayCDA(FILE *,CDA *items);

#endif

