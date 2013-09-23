#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fib-heap.h"

void print_heap(heap *h);
void print_node(node *n);

node *link(node *n1, node *n2) {
    if (n1 == NULL) return n2;
    if (n2 == NULL) return n1;

    // For simplicity, we assume that h1 has the lowest priority key, and if
    // this is not the case, we swap the pointers, so it becomes the case.
    if (n2->key < n1->key) {
        // swap h1 and h2
        node *tmp = n1;
        n1 = n2;
        n2 = tmp;
    }

    // Update the pointers between tree roots
    n2->right_sibling->left_sibling = n2->left_sibling;
    n2->left_sibling->right_sibling = n2->right_sibling;

    n2->parent = n1;
    n1->rank += 1;
    if (n1->child == NULL) {
        // h1 has no child, so h2 becomes only child
        n1->child = n2;
        n2->left_sibling = n2;
        n2->right_sibling = n2;
    } else {
        // h1 has children, so we need to update pointers between them
        node *child1 = n1->child;
        node *child2 = child1->left_sibling;

        n2->left_sibling = child2;
        n2->right_sibling = child1;

        child1->left_sibling = n2;
        child2->right_sibling = n2;
    }

    return n1;
}

node *meld(node *n1, node *n2) {
    if (n1 == NULL) return n2;
    if (n2 == NULL) return n1;

    n1->right_sibling->left_sibling = n2->left_sibling;
    n2->left_sibling->right_sibling = n1->right_sibling;

    n1->right_sibling = n2;
    n2->left_sibling = n1;

    return n1->key < n2->key ? n1 : n2;
}

heap *make_heap() {
    heap *h = malloc(sizeof(heap));
    h->root = NULL;
    h->size = 0;
    return h;
}

node find_min(heap *h) {
    return *h->root;
}

void insert(int key, heap *h) {
    node *n = malloc(sizeof(node));
    n->parent        = NULL;
    n->child         = NULL;
    n->left_sibling  = n;
    n->right_sibling = n;
    n->key           = key;
    n->marked        = 0;
    n->rank          = 0;

    h->root = meld(h->root, n);
    h->size += 1;
}

node *delete_min(heap *h) {
    node *min = h->root;

    if (min->child != NULL) {
        // Null child parents (rootify)
        node *first = min->child;
        node *child = min->child;
        do {
            child->parent = NULL;
            child = child->right_sibling;
        } while (child != first);

        // Insert children into root chain (forrestify)
        if (min->left_sibling != min) {
            // Update root sibling pointers
            min->left_sibling->right_sibling = min->child;
            min->right_sibling->left_sibling = min->child->left_sibling;

            // Update children pointers
            min->child->right_sibling = min->left_sibling;
            min->child->left_sibling->right_sibling = min->right_sibling;
        }
    } else {
        // If there are no children we have to update the sibling pointers
        min->right_sibling->left_sibling = min->left_sibling;
        min->left_sibling->right_sibling = min->right_sibling;
    }

    // LINKING STEP
    // Create an array with max_rank entries and initialize to NULL
    int max_rank = floor(log2(h->size)) + 1; // Max possible rank
    node *seen_ranks[max_rank];
    for (int i = 0; i < max_rank+1; i++) seen_ranks[i] = NULL;

    // Run through all roots and link equal ranked trees
    node *start   = min->right_sibling;
    node *current = min->right_sibling;
    node *new_min = min->right_sibling;

    do {
        // Remember the next node
        node *next = current->right_sibling;

        int cur_rank = current->rank;
        while (seen_ranks[cur_rank] != NULL) {
            current = link(current, seen_ranks[cur_rank]);

            // If the start node is no longer a root we need to update it
            // or the while loop will run indefinitely
            if (start == seen_ranks[cur_rank]) {
                start = current;
            }

            seen_ranks[cur_rank] = NULL;
            cur_rank = current->rank;
        }

        // Update new min if current has a smaller key
        if (current->key < new_min->key) {
            new_min = current;
        }

        // Insert the current node into the array and proceed to next node
        seen_ranks[current->rank] = current;
        current = next;
    } while (start != current);

    // Update the heap
    h->root = new_min;
    h->size -= 1;

    // Return the deleted node
    return min;
}

/**
 * Recursively cut nodes and meld them into the forest
 */
node *cut(heap *h, node *n) {
    if (n->parent == NULL) return h->root;

    // Fix parent child pointer
    if (n->parent->child == n) {
        n->parent->child = (n->right_sibling != n) ? n->right_sibling : NULL;
    }

    // Decrease rank of parent
    n->parent->rank -= 1;

    // Update sibling pointers
    n->right_sibling->left_sibling = n->left_sibling;
    n->left_sibling->right_sibling = n->right_sibling;
    n->left_sibling = n->right_sibling = n;

    // Mark the parent
    if (n->parent->marked) {
        // The parent has already been marked, cut it recursively
        n->parent->marked = 0;
        h->root = cut(h, n->parent);
    } else {
        n->parent->marked = 1;
    }

    // Nullify parent pointer
    n->parent = NULL;

    // Meld the node with the heap
    return meld(h->root, n);
}

void decrease_key(heap *h, node *n, int delta) {
    // Decrease the five dollar ass key, before we make change
    n->key -= delta;

    // If the node is a root or heap order is preserved -> bail
    if (n->parent == NULL || n->parent->key <= n->key) return;

    // Update the heap pointer
    h->root = cut(h, n);
}

void print_tree(node *n, int offset) {
    // Special case: empty heap
    if (n == NULL) { 
        printf("Empty heap\n"); 
        return; 
    }

    // Print indentation
    for (int i = 0; i < offset; i++) printf(" ");

    if (n->marked) {
        printf("%d*\n", n->key);
    } else {
        printf("%d\n", n->key);
    }

    node* first_child = n->child;
    if (first_child == NULL) return;

    node* next_child = n->child;
    do {
        print_tree(next_child, offset + 2);
        next_child = next_child->right_sibling;
    } while (next_child != first_child);
}

void print_heap(heap *h) {
    node *first = h->root;
    node *current = h->root;
    int tree_count = 0;
    do {
        printf("Tree %d:\n", tree_count++);
        print_tree(current, 0);
        current = current->right_sibling;
    } while (first != current);
    printf("\n");
}

void print_node(node *n) {
    printf("NODE\n");
    printf("\tkey:\t%d\n", n->key);
    printf("\trank:\t%d\n", n->rank);
    printf("\tchild:\t%d\n", n->child == NULL ? 0 : n->child->key);
    printf("\trsib:\t%d\n", n->right_sibling == NULL ? 0 : n->right_sibling->key);
    printf("\tlsib:\t%d\n", n->left_sibling == NULL ? 0 : n->left_sibling->key);
    printf("\tparent:\t%d\n", n->parent == NULL ? 0 : n->parent->key);
    printf("\tmarked:\t%d\n", n->marked);
}

int main(void) {
    printf("\n========== LE START OF PROGRAMZ ==========\n\n");
    heap *h = make_heap();
    insert(14, h);
    insert(10, h);
    insert(17, h);
    insert(9, h);
    insert(18, h);
    insert(3, h);
    insert(11, h);
    insert(4, h);
    insert(19, h);
    insert(1, h);
    insert(7, h);
    insert(15, h);
    insert(5, h);
    insert(12, h);
    insert(2, h);
    insert(6, h);
    insert(13, h);
    insert(16, h);

    print_heap(h);

    delete_min(h);
    print_heap(h);

    node *n1 = h->root->child->right_sibling->right_sibling->child;
    decrease_key(h, n1, 11);
    print_heap(h);

    node *n2 = h->root->left_sibling->child->right_sibling->right_sibling->child;
    decrease_key(h, n2, 8);
    print_heap(h);

    return 0; 
}
