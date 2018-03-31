/* Author: Alan Manning */
/* Course: CS 201 Data Structures and Algorithms */
/* Assignment 0 */
/* Date: 12 October, 2017 */

/* This is the header file for a process.c function library. */

#ifndef __PROCESS_INCLUDED__
#define __PROCESS_INCLUDED__

#include <stdio.h>
#include "queue.h"
#include "bst.h"

extern QUEUE *toInfix(FILE *);
extern void printInfix(FILE *, QUEUE *);
extern void printPostfix(FILE *, QUEUE *);
extern BST *storeVars(QUEUE *);
extern double calculateTotal(QUEUE *, BST *);
extern QUEUE *toPostfix(QUEUE *);

#endif
