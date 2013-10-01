#ifndef FIB_HEAP_H
#define FIB_HEAP_H

typedef struct node node;
typedef struct heap heap;

struct heap {
    node *root;
    int size;
};

struct node {
    node* parent;           // Parent node
    node* child;            // Any child node
    node* left_sibling;     // Left sibling
    node* right_sibling;    // Right sibling
    int key;                // Priority
    int rank;               // Rank >= 0
    int marked;             // Unmarked = 0, marked = 1
    void *data;             // The data contained by the node
};

heap* make_heap();
node find_min(heap *h);
node* insert(heap *h, int key);
node* delete_min(heap *h);
void decrease_key(heap *h, node *n, int delta);

#endif
