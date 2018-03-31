/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 1 */
/* Date: 12 October, 2017 */
 
/*This is the main file for an infix to postfix calculator.*/

// MATILDA.C

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "stack.h"
#include "queue.h"
#include "bst.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "scanner.h"
#include "process.h"

/* option flags */
int Author = 0;     						// option -v
int Infix = 0;							// option -i
int Postfix = 0;						// option -p
int BinTree = 0;						// option -b
char* Name = 0;     						// Name of File 

/*************** Private Methods ***************/

static int processOptions(int,char **);
static int numberCount(QUEUE *);
void Fatal(char *,...);

/*************** Main ***************/

int main(int argc, char **argv) {
	double result;
	QUEUE *infix = newQUEUE(displaySTRING);
	QUEUE *postfix = newQUEUE(displaySTRING);
	BST *variableTree;

	if (argc == 1) Fatal("%d arguments!\n",argc-1);		// Error argc=1

	if (argc < 2) {
		fprintf(stderr, "usage: %s\n", argv[0]);	// Error argc<2
		exit(1);
	}

	processOptions(argc,argv);				// Process Opts

	if ((argc == 2) && (!Author)) {
		FILE *fp = fopen(Name, "r");
		
		if (fp == NULL) {                       // Empty file
                	fprintf(stderr, "Error. File %s does not exist.\n", Name);
                       	exit(1);
               	}
                         
                infix = toInfix(fp);                    // toInfix call
                         
                fclose(fp);
                         
                postfix = toPostfix(infix);             // toPostfix
                variableTree = storeVars(infix);        // BST creation
		
		if (sizeQUEUE(postfix) != 0) {
	
			if ((numberCount(postfix) != 0) || (sizeBST(variableTree) != 0)) {
				result = calculateTotal(postfix, variableTree);
				fprintf(stdout, "%lf\n", result);
			}
		}

		return 0;
	}

	if (strcmp(Name, "") != 0) {
		if (Infix || Postfix || BinTree) {
			// Open file if these flags are set
			FILE *fp = fopen(Name, "r");

			if (fp == NULL) {			// Empty file
				fprintf(stderr, "Error. File %s does not exist.\n", Name);
				exit(1);
			}

			infix = toInfix(fp);			// toInfix call
	
			fclose(fp);

			postfix = toPostfix(infix);		// toPostfix
			variableTree = storeVars(infix);	// BST creation
		}
	}

	// Print following if appropriate flags are set

	if (Infix) {
		printInfix(stdout, infix);
		printf("\n");
	}

	if (Postfix) {
		printPostfix(stdout, postfix);
		if (sizeQUEUE(postfix) != 0) {
			printf("\n");
		}
	}

	if (BinTree) {
		displayBST(stdout, variableTree);
		printf("\n");
	}

	if (sizeQUEUE(postfix) != 0) {
	
		if ((numberCount(postfix) != 0) || (sizeBST(variableTree) != 0)) {
			result = calculateTotal(postfix, variableTree);
			fprintf(stdout, "%lf\n", result);
		}
	}
        
	if (Author) {
                printf("%s\n", "Alan A. Manning");
                return 0;
        }

	return 0;
}

/*************** Private Method Defniitions ***************/

void Fatal(char *fmt, ...) {
	va_list ap;

    	fprintf(stderr,"An error occured: ");
    	va_start(ap, fmt);
    	vfprintf(stderr, fmt, ap);
    	va_end(ap);

    	exit(-1);

	return;
}

/* only -oXXX  or -o XXX options */

static int processOptions(int argc, char **argv) {
	int argIndex;
	int argUsed;
	int separateArg;
	int printArg;

    	argIndex = 1;

    	while (argIndex < argc && *argv[argIndex] == '-') {
        	/* check if stdin, represented by "-" is an argument */
        	/* if so, the end of options has been reached */
        	if (argv[argIndex][1] == '\0') {
			return argIndex;
		}

        	separateArg = 0;
        	argUsed = 0;
		printArg = 0;

        	if (argv[argIndex][2] == '\0') {
            		separateArg = 1;
            	}

        	switch (argv[argIndex][1]) {
            		/*
             		* when option has an argument, do this
             		*
             		*     examples are -m4096 or -m 4096
             		*
             		*     case 'm':
             		*         MemorySize = atol(arg);
             		*         argUsed = 1;
             		*         break;
             		*
             		*
             		* when option does not have an argument, do this
             		*
             		*     example is -a
             		*
             		*     case 'a':
             		*         PrintActions = 1;
             		*         break;
             		*/

            		case 'i':
				Infix = 1;
                		argUsed = 1;
                		break;
            		case 'v':
                		Author = 1;
				printArg = 1;
                		break;
			case 'p':
				Postfix = 1;
				argUsed = 1;
				break;
			case 'b':
				BinTree = 1;
				argUsed = 1;
				break;
            		default:
                		Fatal("option %s not understood\n",argv[argIndex]);
            	}

		if (printArg) {
			++argIndex;
		}

        	if (separateArg && argUsed) {
            		++argIndex;
		}
     	
	}
	
	if (argIndex == argc) {
		Name = malloc(sizeof(char) * strlen(argv[argIndex - 1]));
		strcpy(Name, argv[argIndex - 1]); 
	}

	else {
		Name = malloc(sizeof(char) *strlen(argv[argIndex]));
		strcpy(Name, argv[argIndex]);
	}

	return argIndex;
}

/* numberCount method */
/* Counts amount of numbers in postfix queue. */

static int numberCount(QUEUE *postfixQUEUE) {
        int count = 0;
	int i = 0;
	int queueSize = sizeQUEUE(postfixQUEUE);
        QUEUE *temp = newQUEUE(displaySTRING);

        for (i = 0; i < queueSize; ++i) {
                char *x = getSTRING((STRING *) dequeue(postfixQUEUE));
                enqueue(temp, newSTRING(x));

                if (isdigit(x[0])) {
                        ++count;
                }

                else if (((x[0] == '-') || (x[0] == '.')) && (strcmp(x, ";") != 0)) {
                        if (isdigit(x[1])) {
                                ++count;
                        }
                }

        }

	while (sizeQUEUE(temp) != 0) {
                char *y = getSTRING((STRING *) dequeue(temp));
                enqueue(postfixQUEUE, newSTRING(y));
        }

        free(temp);

        
        return count;
}
