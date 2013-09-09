#include <stdio.h>
#include <stdlib.h>

typedef struct heap heap;
typedef struct node node;
heap *make_heap();
node find_min(heap *h);
void insert(int key, heap *h);
void delete_min(heap *h);
void decrease_key(heap *h, node *n, int delta);
node* rotate_up(node *n);
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
        n = rotate_up(n);
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
    root->left_child->parent = last;                        // 5
    root->right_child->parent = last;                       // 6

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

/**
 * Returns a pointer to the nodes new location
 */
node* rotate_up(node *n) {
    node *parent = n->parent;
    node *lc = n->left_child;
    node *rc = n->right_child;

    // Update pointers from the two nodes (6 pointers)
    n->parent = parent->parent;                         // 1
    if (n == parent->left_child) {
        n->left_child = n;                              // 2
        n->right_child = parent->right_child;           // 3
    } else {
        n->left_child = parent->left_child;             // 2
        n->right_child = n;                             // 3
    }

    parent->parent = parent;                            // 4
    parent->left_child = lc;                            // 5
    parent->right_child = rc;                           // 6

    // Switch node and parent
    node *temp = malloc(sizeof(node));
    *temp = *n;
    *n = *parent;
    *parent = *temp;

    return parent;
}

void sift_down(node *n) {
    node *rc = n->right_child;
    node *lc = n->left_child;

    // Return if there is no children (if lc is NULL then rc is NULL)
    if (lc == NULL) return;

    // Guess that we should sift n down left
    node *child_ptr = lc;

    if (n->key < lc->key) {
        // Left child's key is greater than n's key
        if (lc == NULL) return;
        if (rc->key > n->key) return;

        // Switch with right child
        child_ptr = rc;
    } else {
        // Left key is less than n's key
        if (rc != NULL && lc->key > rc->key) {
            // Switch with right child
            child_ptr = rc;
        }
    }

    n = rotate_up(child_ptr);
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
    printf("\n\n");
}

int main() {
    heap *h = make_heap();
    insert(5, h);
    print(h);
    insert(4, h);
    print(h);
    insert(10, h);
    print(h);
    insert(1, h);
    print(h);
    insert(7, h);

//    for (int i = 10; i > 0; i--) {
//        insert(i, h);
//    }
//
//    for (int i = 11; i <= 15; i++) {
//        insert(i, h);
//    }


    print(h);
    decrease_key(h, h->root->left_child->right_child, 7);
    print(h);
    delete_min(h);

    print(h);
}
