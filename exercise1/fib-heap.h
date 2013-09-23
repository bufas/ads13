#ifndef FIB_HEAP_H
#define FIB_HEAP_H

typedef struct node node;
typedef struct heap heap;

struct heap {
    node *root;             // Min element of the heap
    int size;               // Size of the heap
};

struct node {
    node* parent;           // Parent node
    node* child;            // Any child node
    node* left_sibling;     // Left sibling
    node* right_sibling;    // Right sibling
    int key;                // Priority
    int rank;               // Rank >= 0
    int marked;             // Unmarked = 0, marked = 1
};

/**
 * Returns a null pointer as that is what an empty heap is
 * @return a new empty heap (null pointer)
 */
heap *make_heap();

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
void insert(int key, heap *h);

/**
 * Deletes the minimum element from a given heap
 * @param h the heap on which to carry out the action
 */
node *delete_min(heap *h);

/**
 * Decreases the key by the amount delta of node n
 * @param h the heap on which to carry out the action
 * @param n the node whos key should be decreased
 * @param delta the amount to decrease the key
 */
void decrease_key(heap *h, node *n, int delta);

#endif
