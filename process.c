/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 1 */
/* Date: 12 October, 2017 */

/*This is the implementation file for the header file "process.h",
 which is used to create a library of methods for processing
 data used in an infix-to-postfix calculator.*/

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

/*************** Private Interface ***************/

static int precedence(char *);
static bool hasHigherPrec(char *, char *);
static bool isOpenParen(char *);
static bool isCloseParen(char *);
static void d(FILE *, void *, void *);
static int c(void *, void *);

/*************** Public Method Definitions ***************/

/* toInfix converter */
/* Reads strings from a file and converts them 
to infix notation. */

QUEUE *toInfix(FILE *fp) {
	char *x;
	QUEUE *infixQUEUE = newQUEUE(displaySTRING);

	while(!feof(fp)) {
		x = readToken(fp);				// Read tokens
		if (x != '\0') {
			enqueue(infixQUEUE, newSTRING(x));	// Add to infix
		}
	}
	
	return infixQUEUE;
}

/* toPostfix converter */
/* Reads tokens from infix queue and converts
them to postfix queue. */

QUEUE *toPostfix(QUEUE *infixQUEUE) {
	int i = 0;
	QUEUE *postfixQUEUE = newQUEUE(displaySTRING);
	QUEUE *temp = newQUEUE(displaySTRING);			// Dummy var
	STACK *operators = newSTACK(displaySTRING);

	while (sizeQUEUE(infixQUEUE) != 0) {
		char *x = getSTRING((STRING *) dequeue(infixQUEUE));
		enqueue(temp, newSTRING(x));
		int operation = precedence(x);

		// Skips variable declarations	
		if (strcmp(x, "var") == 0) {
			for (i = 0; i < 4; ++i) {
				char *x = getSTRING((STRING *) dequeue(infixQUEUE));
				enqueue(temp, newSTRING(x));
			}
			continue;	
		}

		// Case if token is operator
                if (operation >= 0) {
                	// Higher precedence and not opening parentheses
                        while ((sizeSTACK(operators) != 0) && !isOpenParen(getSTRING((STRING *) peekSTACK(operators))) && hasHigherPrec(getSTRING((STRING *) peekSTACK(operators)), x)) {
                        	enqueue(postfixQUEUE, newSTRING(getSTRING((STRING *) peekSTACK(operators))));
                                pop(operators);
                        }
 
			push(operators, newSTRING(x));
		}

                // Case if token is '('
                else if (isOpenParen(x)) {
                	push(operators, newSTRING(x));
                }

		// Case if token is ')'
                else if (isCloseParen(x)) {
                	while ((sizeSTACK(operators) != 0) && !isOpenParen(getSTRING((STRING * ) peekSTACK(operators)))) {
                        	enqueue(postfixQUEUE, newSTRING(getSTRING((STRING *) peekSTACK(operators))));
                        	pop(operators);
                        }

                        pop(operators);
		}
                        
                // Case if char or number is operand
                else if ((strcmp(x, ";") != 0)) {
                	enqueue(postfixQUEUE, newSTRING(x));
                }
	}

	// Remove remaining items from stack
        while (sizeSTACK(operators) > 0) {
        	enqueue(postfixQUEUE, newSTRING(getSTRING((STRING *) peekSTACK(operators))));
                pop(operators);
        }

	// Replenish infix queue
	while (sizeQUEUE(temp) != 0) {
                char *y = getSTRING((STRING *) dequeue(temp));
                enqueue(infixQUEUE, newSTRING(y));
        }

	free(temp);

	return postfixQUEUE;
}

/* printInfix Method */
/* Prints infix notation. */

void printInfix(FILE *fp, QUEUE *infixQUEUE) {
	int i = 0;
	int infixSize = sizeQUEUE(infixQUEUE);
	QUEUE *temp = newQUEUE(displaySTRING);

        for (i = 0; i < infixSize; ++i) {
		char *x = getSTRING((STRING *) dequeue(infixQUEUE));
		enqueue(temp, newSTRING(x));
                fprintf(fp, "%s", x);

		// Print newline after semicolon
		if ((strcmp(x, ";") == 0) && (sizeQUEUE(infixQUEUE) != 0)) {
			fprintf(fp, "\n");
			continue;
		}
                
		// Adjust spacing
                if (sizeQUEUE(infixQUEUE) != 0) {
                        fprintf(fp, " ");
                }
        }

	while (sizeQUEUE(temp) != 0) {
		char *z = getSTRING((STRING *) dequeue(temp));
		enqueue(infixQUEUE, newSTRING(z));	
	}

	free(temp);

	return;
}

/* printPostfix method */
/* Prints postfix notation. */

void printPostfix(FILE *fp, QUEUE *postfixQUEUE) {
	int i = 0;
	int postfixSize = sizeQUEUE(postfixQUEUE);
	QUEUE *temp = newQUEUE(displaySTRING);

        for (i = 0; i < postfixSize; ++i) {
		char *x = getSTRING((STRING *) dequeue(postfixQUEUE));
		enqueue(temp, newSTRING(x));
                fprintf(fp, "%s", x);

		// Adjust spacing
                if (sizeQUEUE(postfixQUEUE) != 0) {
                        fprintf(fp, " ");
                }
        }

	while (sizeQUEUE(temp) != 0) {
                char *y = getSTRING((STRING *) dequeue(temp));
                enqueue(postfixQUEUE, newSTRING(y));
        }

	free(temp);

	return;
}

/* storeVars Method */
/* Stores values in BST. */

BST *storeVars(QUEUE *infixQUEUE) {
	double value = 0.0;
	BST *varTree = newBST(d, c);
	QUEUE *temp = newQUEUE(displaySTRING);

	// Search for 'var' declaration and store value and key
	while (sizeQUEUE(infixQUEUE) != 0) {
		char *var = getSTRING((STRING *) dequeue(infixQUEUE));
		enqueue(temp, newSTRING(var));
		
		if (strcmp(var, "var") == 0) {
			char *key = getSTRING((STRING *) dequeue(infixQUEUE));
			enqueue(temp, newSTRING(key));
		
			// Discard "="	
			char *equal = getSTRING((STRING *) dequeue(infixQUEUE));
			enqueue(temp, newSTRING(equal));

			char *valueString = getSTRING((STRING *) dequeue(infixQUEUE));
			enqueue(temp, newSTRING(valueString));

			value = atof(valueString);
			insertBST(varTree, newSTRING(key), newREAL(value));

			continue;
		}
	
		// Error handling for undeclared variable	
		else if ((strcmp(var, "var") != 0) && (isalpha(var[0])) && (findBST(varTree, newSTRING(var)) == NULL)) {
			fprintf(stdout, "variable %s was not declared\n", var);
			exit(0);
		}
	}

	while (sizeQUEUE(temp) != 0) {
		char *y = getSTRING((STRING *) dequeue(temp));
		enqueue(infixQUEUE, newSTRING(y));
	}

	free(temp);

	return varTree;
}

/* calculateTotal Method */
/* Calculates total from postfix queue and BST. */

double calculateTotal(QUEUE *postfixQUEUE, BST *varTree) {
	int i = 0;
	int queueSize = sizeQUEUE(postfixQUEUE);
	double answer = 0.0;
	double bstValue = 0.0;
	QUEUE *temp = newQUEUE(displaySTRING);
	STACK *calculations = newSTACK(displayREAL);

	// Calculate total
	for (i = 0; i < queueSize; ++i) {                       
        	char *key = getSTRING((STRING *) dequeue(postfixQUEUE));
		enqueue(temp, newSTRING(key));
		int operation = precedence(key);		// Precedence
        
		// Token is number or variable       
        	if ((operation < 0)) {
			STRING *temp = newSTRING(key);

			// Number found in BST	
			if (findBST(varTree, temp) != 0) {
				bstValue = getREAL((REAL *) findBST(varTree, temp));
			}

			// Number not found in BST
			else {
				bstValue = atof(key);
			}

                	push(calculations, newREAL(bstValue));
                }

		// Token is operator
                else {
                        int operation = precedence(key);
                        double secondValue = getREAL((REAL *) pop(calculations));
                        double firstValue = getREAL((REAL *) pop(calculations));

                        double total = 0.0;

				// Calculate based on precedence
                                switch(operation) {
                                        case 0:
                                                total = firstValue + secondValue;
                                                break;
        
                                        case 1:
                                                total = firstValue - secondValue;
                                                break;
                                
                                        case 2:
                                                total = firstValue * secondValue;
                                                break;
        
                                        case 3:
						if (secondValue == 0) {
							fprintf(stdout, "Error: Division by zero. Exiting.\n");
							exit(0);
						}
                                                total = firstValue / secondValue;
                                                break;

                                        case 4:
                                                total = fmod(firstValue, secondValue);
                                                break;

                                        case 5:
                                                total = pow(firstValue, secondValue);
                                                break;

                                        default:
                                                assert(0);
                                }

                        push(calculations, newREAL(total));
                }
        }

	while (sizeQUEUE(temp) != 0) {
		char *y = getSTRING((STRING *) dequeue(temp));
		enqueue(postfixQUEUE, newSTRING(y));
	}	

	answer = getREAL((REAL *) pop(calculations));
	
	free(temp);

	return answer;
}

/*************** Private Method Definitions ***************/

/* hasHigherPrec Method */
/* Determines if the first token has higher
precedence than the other. */

static bool hasHigherPrec(char *token1, char *token2) {
	bool higher = false;

	if (precedence(token1) >= precedence(token2)) {
		higher = true;
	}

	return higher;
}

/* isOpenParen Method */
/* Determines if token is open parenthesis. */

static bool isOpenParen(char *token1) {
	bool parenthesis = false;

	int type1 = strcmp(token1, "(");
	int type2 = strcmp(token1, "[");
	int type3 = strcmp(token1, "{");

	if ((!type1) || (!type2) || (!type3)) {
		parenthesis = true;
	}

	return parenthesis;
}

/* isCloseParen Method */
/* Determines if token is closed parenthesis. */

static bool isCloseParen(char *token1) {
	bool parenthesis = false;

	int type1 = strcmp(token1, ")");
	int type2 = strcmp(token1, "]");
	int type3 = strcmp(token1, "}");

	if (!type1 || !type2 || !type3) {
		parenthesis = true;
	}

	return parenthesis;
}

/* precedence method */
/* Determines token precedence. */

static int precedence(char *token) {
	int associate = 0;

	int type0 = strcmp(token, "+");
	int type1 = strcmp(token, "-");
	int type2 = strcmp(token, "*");
	int type3 = strcmp(token, "/");
	int type4 = strcmp(token, "%");
	int type5 = strcmp(token, "^");

	if (type0 == 0) associate = 0;
	else if (type1 == 0) associate = 1;
	else if (type2 == 0) associate = 2;
	else if (type3 == 0) associate = 3;
	else if (type4 == 0) associate = 4;
	else if (type5 == 0) associate = 5;
	else if (isOpenParen(token)) associate = -1;
	else associate = -2;

	return associate;
}

/* display method */
/* Displays key and value for BST entry. */

static void d(FILE *fp, void *key, void *value) {
	fprintf(fp, "%s=%lf", getSTRING((STRING *) key), getREAL((REAL *) value));

	return;
}

/* comparator method */
/* Compares key entries for BST using strcmp method */

static int c(void *key1, void *key2) {
	char *firstKey = getSTRING((STRING *) key1);
	char *secondKey = getSTRING((STRING *) key2);

	int result = strcmp(firstKey, secondKey);

	return result;
}
