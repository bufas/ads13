#include <stdio.h>
#include <stdlib.h>

typedef struct heap heap;
typedef struct node node;
heap *make_heap();
node find_min(heap *h);
void insert(int key, heap *h);
void delete_min(heap *h);
void bubble_up(heap *h, node *n);
void print(heap *h);
void print_aux(node *n, int indent);

struct heap {
    node *root;
    int size;
};
struct node {
    node *parent;           // Parent node
    node *left_child;       // Left child
    node *right_child;      // Right child
    int key;                // Priority
};

heap *make_heap() {
    heap *h = malloc(sizeof(heap));
    h->root = NULL;
    h->size = 0;
    return h;
}

node find_min(heap *h) {
    return *(h->root);
}

void insert(int key, heap *h) {
    node *n = malloc(sizeof(node));
    n->left_child = NULL;
    n->right_child = NULL;
    n->key = key;

    if (h->root == NULL) {
        // This is the first element
        n->parent = NULL;
        h->root = n;
    } else {
        // Find the position to place this node under
        // Go left for every 0 after the first 1
        // Go right for every 1 after the first 1
        int seenOne = 0;
        node *parent = h->root;
        for (int i = 30; i >= 0; i -= 1) {
            int bitN = ((h->size + 1) >> i) & 1;

            if (seenOne && !bitN) {
                if (parent->left_child == NULL) {
                    // Insert here!
                    n->parent = parent;
                    parent->left_child = n;
                    break;
                } else {
                    // We need to go further
                    parent = parent->left_child;
                }
            }

            if (seenOne && bitN) {
                if (parent->right_child == NULL) {
                    // Insert here!
                    n->parent = parent;
                    parent->right_child = n;
                    break;
                } else {
                    // We need to go further
                    parent = parent->right_child;
                }
            }

            if (!seenOne && bitN) seenOne = 1;
        }
    }

    bubble_up(h, n);

    h->size += 1;
}

void bubble_up(heap *h, node *n) {
    while (n->parent != NULL && n->parent->key > n->key) {
        // Set variables
        node *parent = n->parent;
        node *left_child = n->left_child;
        node *right_child = n->right_child;
        node *grandparent = NULL;
        if(parent != NULL) grandparent = n->parent->parent;

        // Swap n with its parent, there are 10 pointers to update
        if(left_child != NULL) left_child->parent = parent;   // 1
        if(right_child != NULL) right_child->parent = parent; // 2

        // Find out if n is the right or left child
        node *sibling = NULL;
        if (n == parent->left_child) {
            // n is the left child
            sibling = parent->right_child;
            n->left_child = parent;                           // 4
            n->right_child = sibling;                         // 5
        } else {
            // n is the right child
            sibling = parent->left_child;
            n->left_child = sibling;                          // 4
            n->right_child = parent;                          // 5
        }

        if (sibling != NULL) sibling->parent = n;             // 3

        // Find out if parent is right or left child
        if(grandparent != NULL) { 
            if (parent == grandparent->left_child) {
                // parent is left child
                grandparent->left_child = n;                  // 6
            } else {
                // parent is right child
                grandparent->right_child = n;                 // 6
            }
        }

        parent->left_child = left_child;                      // 7
        parent->right_child = right_child;                    // 8

        parent->parent = n;                                   // 9
        n->parent = grandparent;                              // 10
    }

    // If n got to the top, update the root
    if (n->parent == NULL) {
        h->root = n;
    }
}

void print_aux(node *n, int indent) {
    if (n == NULL) return;
    for (int i = 0; i < indent; i += 1) printf(" ");
    printf("%d\n", n->key);
    print_aux(n->left_child, indent + 3);
    print_aux(n->right_child, indent + 3);
}

void print(heap *h) {
    print_aux(h->root, 0);
}

int main() {
    heap *h = make_heap();
    insert(15, h);
    insert(5, h);
    insert(10, h);
    insert(20, h);

    print(h);
}
