#include <stdio.h>
#include <stdlib.h>
#include "fib-heap.h"

heap meld(heap h1, heap h2) {
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

    printf("Find out stuff %d %d\n", h1->key, h2->key);

    h2->parent = h1;
    h1->rank += 1; // TODO dont't know if this is correct
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

void delete_min(heap *h) {
    heap min = *h;

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

    // Linking step
    // Find two trees with equal rank
    // DO ME!
}

void print_heap(heap h, int offset) {
    // Print indentation
    for (int i = 0; i < offset; i++) printf(" ");

    // Special case: empty heap
    if (h == NULL) { printf("Empty heap\n"); return; }


    printf("%d\n", h->key);

    node* first_child = h->child;
    if (first_child == NULL) return;

    node* next_child = h->child;
    do {
        print_heap(next_child, offset + 2);
        next_child = next_child->right_sibling;
    } while (next_child != first_child);
}

int main(void) {
    printf("\n========== LE START OF PROGRAMZ ==========\n\n");
    heap h = make_heap();
    insert(5, &h);
    insert(9, &h);
    insert(3, &h);
    insert(1, &h);
    insert(4, &h);
    insert(7, &h);
    insert(2, &h);
    insert(6, &h);

    print_heap(h, 0);

    return 0; 
}
