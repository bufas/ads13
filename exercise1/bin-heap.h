#ifndef BIN_HEAP_H
#define BIN_HEAP_H

typedef struct heap heap;
typedef struct node node;

struct heap {
    node *root;         // A pointer to the root nodea (minimum)
    int size;           // Holds the size of the heap
};

struct node {
    node *parent;       // Parent node
    node *left_child;   // Left child
    node *right_child;  // Right child
    int key;            // Priority
    void *data;         // The data contained by the node
};

/**
 * Creates a new empty heap
 * @return a pointer to the new heap
 */
heap* make_heap();

/**
 * Returns the minimum element of the heap
 * h a pointer to the heap
 * @return the minimum node of the heap h
 */
node find_min(heap *h);

/**
 * Inserts an element with a specified key into
 * a give heap
 * @param key the key the element new element should have
 * @param h the heap on which to carry out the action
 */
node* insert(heap *h, int key);

/**
 * Deletes the minimum element from a given heap
 * @param h the heap on which to carry out the action
 */
node* delete_min(heap *h);

/**
 * Decreases the key by the amount delta of node n
 * @param h the heap on which to carry out the action
 * @param n the node whos key should be decreased
 * @param delta the amount to decrease the key
 */
void decrease_key(heap *h, node *n, int delta);

#endif