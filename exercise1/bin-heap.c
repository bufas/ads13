#include <stdio.h>
#include <stdlib.h>

typedef struct heap heap;
typedef struct node node;
heap *make_heap();
node find_min(heap *h);
void insert(int key, heap *h);
void delete_min(heap *h);
void decrease_key(heap *h, node *n, int delta);
void rotate_up(node *n);
void sift_down(node *n);
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

void decrease_key(heap *h, node *n, int delta) {
    n->key -= delta;
    bubble_up(h, n);
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

void delete_min(heap *h) {
    // Keep a pointer to the min element
    node *root = h->root;

    // Find the last node
    int seenOne = 0;
    node *last = h->root;
    int bitN;
    for (int i = 30; i >= 0; i -= 1) {
        bitN = (h->size >> i) & 1;

        // Go left or right depending on the bit value
        if (seenOne && !bitN) last = last->left_child;
        if (seenOne && bitN)  last = last->right_child;

        if (!seenOne && bitN) seenOne = 1;
    }

    // Update the last nodes parents child pointer to NULL
    if (bitN) last->parent->right_child = NULL;             // 1
    else      last->parent->left_child = NULL;              // 1

    // Set the last node as the new root
    last->left_child = root->left_child;                    // 2
    last->right_child = root->right_child;                  // 3
    last->parent = NULL;                                    // 4

    // Update old root's children's parent pointers
    root->left_child->parent = last;
    root->right_child->parent = last;

    // Update the heap fields
    h->size -= 1;

    // Restore heap order
    sift_down(last);

    // update root by following parent pointers
    node *new_root = last;
    while (new_root->parent != NULL) {
        new_root = new_root->parent;
    }
    h->root = new_root;

    // Free Willy Wonka
    free(root);
}

void rotate_up(node *n) {
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
    if (parent == NULL) printf("parent is null\n");
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

void sift_down(node *n) {
    // Return if there is no children (if left child is NULL then right child is NULL)
    if (n->left_child == NULL) return;

    if (n->key < n->left_child->key) {
        // Left child's key is greater than n's key
        if (n->right_child == NULL) return;
        if (n->right_child->key > n->key) return;

        // Switch with right child
        rotate_up(n->right_child);
    } else {
        // Left key is less than n's key
        if (n->right_child == NULL) {
            // Switch with left child
            rotate_up(n->left_child);
        } else {
            if (n->right_child->key > n->left_child->key) {
                // Switch with left child
                rotate_up(n->left_child);
            } else {
                // Switch with right child
                rotate_up(n->right_child);
            }
        }
    }

    sift_down(n);
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
    for (int i = 10; i > 0; i--) {
        insert(i, h);
    }
    for (int i = 11; i <= 15; i++) {
        insert(i, h);
    }

    decrease_key(h, h->root->left_child->right_child, 3);
    delete_min(h);

    print(h);
}
