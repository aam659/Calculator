/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 1 */
/* Date: 28 September, 2017 */

/*This is the implementation file for the header file "bst.h",
which is used to create a binary search tree 'class' for
generic data types.*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include "bst.h"

// NODE Definition
typedef struct NODE NODE;

struct NODE {
	NODE *left, *right, *par;

	void *k, *val;
};

struct BST {
	void (*disp)(FILE *, void *, void *);			// display field
	int (*compare)(void *, void *);				// comparator method for keys
	int size;						// tree size
	NODE *root;
};

/*************** private interface ***************/
static void inorder(FILE *, BST *, NODE *);
static bool hasLeft(NODE *);
static bool hasRight(NODE *);
static NODE *newNODE(void *, void *);

/*************** public interface ***************/

/* constructor */
/* This is the constructor for the bst objects. */

BST *newBST(void (*display)(FILE *,void *,void *),int (*comparator)(void *,void *)) {
	BST *tree = malloc(sizeof(BST));			// Allocate space for BST object

	assert(tree != 0);

	tree->size = 0;						// Set BST size to 0
	tree->disp = display;					// disp field = display method
	tree->compare = comparator;				// compare field = comparator method

	tree->root = NULL;					// Set root to NULL

	return tree;
}

/* insert method */

void insertBST(BST *tree,void *key,void *value) {
	NODE *leaf = newNODE(key, value);

	if (tree->root == NULL) {				// Insert at root
		leaf->par = NULL;
		tree->root = leaf;
		tree->size += 1;
	}

	else {
		NODE *cur = tree->root;

		while (cur != NULL) {
			if (tree->compare(key, cur->k) < 0) {
				if (cur->left == NULL) {	// Left child insert
					cur->left = leaf;
					leaf->par = cur;
					cur = NULL;
					tree->size += 1;
				}

				else {
					cur = cur->left;	// Traverse left
				}
			}
		
			else if (tree->compare(key, cur->k) > 0) {
				if (cur->right == NULL) {	// Right child insert
					cur->right = leaf;
					leaf->par = cur;
					cur = NULL;
					tree->size += 1;
				}

				else {
					cur = cur->right;	// Traverse right
				}
			}

			else {
				return;				// Prevents duplicate nodes	
			}
		}
	}

	return;
}

/* search method */
/* This method searches for a node value, based on its key value. */

void *findBST(BST *tree,void *key) {
	if (tree->root == NULL) {
		return NULL;					// EMPTY List
	}	

	else {
		NODE *cur = tree->root;

		while (cur != NULL) {				// While not NULL, traverse tree
			if (tree->compare(key, cur->k) == 0) {
				return cur->val;		// Match returns node
			}

			else if (tree->compare(key, cur->k) < 0) {
				cur = cur->left;		// Traverse left
			}

			else {
				cur = cur->right;		// Traverse right
			}
		}
	}

	return NULL;						// Return NULL for void * return type
}

/* size method */
/* This is the method that returns the tree size. */

int sizeBST(BST *tree) {
	return tree->size;					// Return tree size
}

/* display method */
/* This method displays the values in the BST, using
an in-order traversal. For any given node, it shows the
left and right subtrees, each surrounded by brackets, if
these exist. A space is put between an existing subtree
and the node value. An empty tree is []. */

void displayBST(FILE *fp,BST *tree) {
	if (tree->root == NULL) {
		fprintf(fp, "[]");				// Empty tree
		return;
	}

	inorder(fp, tree, tree->root);

	return;	
}

/*************** private method definitions ***************/

/* In-order Traversal Method */
/* This method performs an in-order traversal 
for the given tree. */

static void inorder(FILE *fp, BST *tree, NODE *node) {
        if (node != NULL) {
                fprintf(fp, "[");
                inorder(fp, tree, node->left);                  // Traverse left
                if (hasLeft(node)) {
                        fprintf(fp, " ");			// Space if left-child
                }
                tree->disp(fp, node->k, node->val);             // Cached display prints node->val
		if (hasRight(node)) {
			fprintf(fp, " ");			// Space if right-child
		}
                inorder(fp, tree, node->right);                 // Traverse right
                fprintf(fp, "]");
        }

        return;
}

static bool hasLeft(NODE *node) {
        bool leftChild = false;

        if (node->left != NULL) {
                leftChild = true;
        }

        return leftChild;
}

static bool hasRight(NODE *node) {
        bool rightChild = false;

        if (node->right != NULL) {
                rightChild = true;
        }

        return rightChild;
}

static NODE *newNODE(void *key, void *value) {
	NODE *leaf = malloc(sizeof(NODE));			// Allocate space for BST object

	assert(leaf != 0);
	leaf->k = key;
	leaf->val = value;
	leaf->left = leaf->right = NULL;

	return leaf;
}
