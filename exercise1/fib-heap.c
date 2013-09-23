#include <stdio.h>
#include <stdlib.h>
#include "fib-heap.h"

void print_heap(heap h);
void print_node(node *n);

heap link(heap h1, heap h2) {
    if (h1 == NULL) return h2;
    if (h2 == NULL) return h1;

    // For simplicity, we assume that h1 has the lowest priority key, and if
    // this is not the case, we swap the pointers, so it becomes the case.
    if (h2->key < h1->key) {
        // swap h1 and h2
        heap tmp = h1;
        h1 = h2;
        h2 = tmp;
    }

    // Update the pointers between tree roots
    h2->right_sibling->left_sibling = h2->left_sibling;
    h2->left_sibling->right_sibling = h2->right_sibling;

    h2->parent = h1;
    h1->rank += 1;
    if (h1->child == NULL) {
        // h1 has no child, so h2 becomes only child
        h1->child = h2;
        h2->left_sibling = h2;
        h2->right_sibling = h2;
    } else {
        // h1 has children, so we need to update pointers between them
        node *child1 = h1->child;
        node *child2 = child1->left_sibling;

        h2->left_sibling = child2;
        h2->right_sibling = child1;

        child1->left_sibling = h2;
        child2->right_sibling = h2;
    }

    return h1;
}

heap meld(heap h1, heap h2) {
    if (h1 == NULL) return h2;
    if (h2 == NULL) return h1;

    h1->right_sibling->left_sibling = h2->left_sibling;
    h2->left_sibling->right_sibling = h1->right_sibling;

    h1->right_sibling = h2;
    h2->left_sibling = h1;

    return h1->key < h2->key ? h1 : h2;
}

heap make_heap() {
    return NULL;
}

node find_min(heap h) {
    return *h;
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

    *h = meld(*h, n);
}

node *delete_min(heap *h) {
    heap min = *h;

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

    // Linking step
    heap anchor = min->right_sibling;
    // Run through all roots to get the biggest rank
    int max_rank = anchor->rank;
    int no_of_trees = 0;
    heap i = anchor->right_sibling;
    while (i != anchor) {
        max_rank = i->rank > max_rank ? i->rank : max_rank;
        i = i->right_sibling;
        no_of_trees++;
    }
    // Create an array with max_rank entries and initialize to NULL
    int array_size = max_rank + no_of_trees + 1;
    heap seen_ranks[array_size];
    for (int i = 0; i < array_size; i++) seen_ranks[i] = NULL;
    // Run through all roots and link equal ranked trees
    heap start = anchor;
    heap current = anchor;
    int first = 1;
    while(current != start || first) {
        int cur_rank = current->rank;
        if (seen_ranks[cur_rank] != NULL) {
            // link these two and set new starting point
            if (current == seen_ranks[cur_rank]) {
                current = current->right_sibling;
                continue;
            }
            current = link(current, seen_ranks[cur_rank]);
            seen_ranks[cur_rank] = NULL;
            start = current;
            first = 1;
        } else {
            // put this heap into the array and move right
            seen_ranks[cur_rank] = current;
            current = current->right_sibling;
            first = 0;
        }
        anchor = current; // This is just to keep a reference to a root node
    }

    // Set h to the new minimum
    heap new_min = anchor;
    i = anchor->right_sibling;
    while (i != anchor) {
        printf("new_min->key = %d, i->key = %d\n", new_min->key, i->key);
        new_min = i->key < new_min->key ? i : new_min;
        i = i->right_sibling;
    }
    *h = new_min;

    return min;
}

/**
 * Recursively cut nodes and meld them into the forest
 */
heap cut(heap h, node *n) {
    if (n->parent == NULL) return h;

    print_node(n);

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
        printf("kartofler\n");
        // The parent has already been marked, cut it recursively
        n->parent->marked = 0;
        h = cut(h, n->parent);
    } else {
        printf("sovs\n");
        n->parent->marked = 1;
    }

    // Nullify parent pointer
    n->parent = NULL;

    print_node(n);

    // Meld the node with the heap
    return meld(h, n);
}

void decrease_key(heap *h, node *n, int delta) {
    // Decrease the five dollar ass key, before we make change
    n->key -= delta;

    // If the node is a root or heap order is preserved -> bail
    if (n->parent == NULL || n->parent->key <= n->key) return;

    // Update the heap pointer
    *h = cut(*h, n);
}

void print_tree(heap h, int offset) {
    // Print indentation
    for (int i = 0; i < offset; i++) printf(" ");

    // Special case: empty heap
    if (h == NULL) { 
        printf("Empty heap\n"); 
        return; 
    }

    if (h->marked) {
        printf("%d*\n", h->key);
    } else {
        printf("%d\n", h->key);
    }

    node* first_child = h->child;
    if (first_child == NULL) return;

    node* next_child = h->child;
    do {
        print_tree(next_child, offset + 2);
        next_child = next_child->right_sibling;
    } while (next_child != first_child);
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

void print_heap(heap h) {
    heap first = h;
    heap current = h;
    int tree_count = 0;
    do {
        printf("Tree %d:\n", tree_count++);
        print_tree(current, 0);
        current = current->right_sibling;
    } while (first != current);
    printf("\n");
}


int main(void) {
    printf("\n========== LE START OF PROGRAMZ ==========\n\n");
    heap h = make_heap();
    insert(14, &h);
    insert(10, &h);
    insert(17, &h);
    insert(9, &h);
    insert(18, &h);
    insert(3, &h);
    insert(11, &h);
    insert(4, &h);
    insert(19, &h);
    insert(1, &h);
    insert(7, &h);
    insert(15, &h);
    insert(5, &h);
    insert(12, &h);
    insert(2, &h);
    insert(6, &h);
    insert(13, &h);
    insert(16, &h);

    print_heap(h);

    delete_min(&h);
    print_heap(h);

    node *n1 = h->child->right_sibling->right_sibling->child;
    decrease_key(&h, n1, 11);
    print_heap(h);

    node *n2 = h->left_sibling->child->right_sibling->right_sibling->child;
    decrease_key(&h, n2, 8);
    print_heap(h);

    return 0; 
}
