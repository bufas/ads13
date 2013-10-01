#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fib-heap.h"

#define DEBUG 0

//=============================================================================
//  Debug functions
//=============================================================================

// Print the given string if debugging
void debug(char *s) {
    if (DEBUG) printf("%s\n", s);
}

// Print the given string as an error and exit
void error(char *s) {
    printf("### ERROR: %s\n", s);
    exit(-1);
}

// Print the given string
void p(char *s) {
    printf("%s\n", s);
}

void assert_siblings_have_same_parent(node *n) {
    debug("Checking if all siblings have same parent...");
    node *p = n->parent;
    node *c = n->right_sibling;
    while (c != n) {
        if (c->parent != p) {
            printf("          (^%p) [%p]\n          (^%p) [%p]\n", c->parent, c, n, p);
            error("BROTHER FROM ANOTHER MOTHER!");
        }
        c = c->right_sibling;
    }
}

void assert_node_has_lowest_key(node *n) {
    if (n == NULL) return;
    int key = n->key;
    node *c = n->right_sibling;
    while (c != n) {
        if (c->key < key) error("ROOT IS NOT LOWEST KEY!");
        c = c->right_sibling;
    }
}

void assert_that(int b, char *msg) {
    debug(msg);
    if (!b) error(msg);
}
void insert_in_empty_heap() {
    heap *h = make_heap();
    assert_that(h->root == NULL, "Root must be null");
    insert(h, 12);
    assert_that(h->root != NULL, "Root must be non-null");
    assert_that(h->root->key == 12, "Root key must be 12");
    free(h);
}

//=============================================================================
//  Test functions
//=============================================================================

void insert_in_non_empty_heap() {
    heap *h = make_heap();
    insert(h, 12);
    assert_node_has_lowest_key(h->root);
    assert_that(h->size == 1, "Size must be 1");
    insert(h, 5);
    assert_node_has_lowest_key(h->root);
    assert_that(h->size == 2, "Size must be 2");
    insert(h, 50);
    assert_node_has_lowest_key(h->root);
    assert_that(h->size == 3, "Size must be 3");

    // Check that duplicates are inserted properly
    insert(h, 50);
    assert_that(h->size == 4, "Size must be 4");
    insert(h, 50);
    assert_that(h->size == 5, "Size must be 5");
    free(h);
}

void delete_min_in_single_node_heap_p1() {
    heap *h = make_heap();
    insert(h, 1);
    assert_that(h->size == 1, "Size must be 1");
    assert_that(h->root != NULL, "Root must be non-null");
    delete_min(h);
    assert_that(h->size == 0, "Size must be 0");
    assert_that(h->root == NULL, "Root must be null");
    free(h);
}

void delete_min_in_multi_node_heap_p1() {
    heap *h = make_heap();
    insert(h, 1);
    insert(h, 5);
    insert(h, 2);
    insert(h, 3);
    insert(h, 1);
    assert_that(h->size == 5, "Size must be 5");
    assert_that(h->root != NULL, "Root must be non-null");
    delete_min(h);
    assert_that(h->size == 4, "Size must be 4");
    assert_that(h->root != NULL, "Root must be non-null");
    delete_min(h);
    assert_that(h->size == 3, "Size must be 3");
    assert_that(h->root != NULL, "Root must be non-null");
    delete_min(h);
    assert_that(h->size == 2, "Size must be 2");
    assert_that(h->root != NULL, "Root must be non-null");
    delete_min(h);
    assert_that(h->size == 1, "Size must be 1");
    assert_that(h->root != NULL, "Root must be non-null");
    delete_min(h);
    assert_that(h->size == 0, "Size must be 0");
    assert_that(h->root == NULL, "Root must be null");
    free(h);
}

void delete_min_in_multi_node_heap_p2() {
    heap *h = make_heap();
    insert(h, 1);
    insert(h, 8);
    insert(h, 8);
    insert(h, 5);
    assert_that(root_count(h) == 4, "Heap must have 4 roots");
    assert_that(h->size == 4, "Size must be 4");
    delete_min(h);
    assert_that(root_count(h) == 2, "Heap must have 2 roots");
    assert_that(h->size == 3, "Size must be 3");
    delete_min(h);
    assert_that(h->size == 2, "Size must be 2");
    delete_min(h);
    assert_that(h->size == 1, "Size must be 1");
    free(h);
}

void delete_min_in_multi_node_heap_p3() {
    heap *h = make_heap();
    insert(h, 1);
    insert(h, 5);
    insert(h, 2);
    insert(h, 3);
    insert(h, 1);
    delete_min(h);
    assert_node_has_lowest_key(h->root);
    delete_min(h);
    assert_node_has_lowest_key(h->root);
    delete_min(h);
    assert_node_has_lowest_key(h->root);
    delete_min(h);
    assert_node_has_lowest_key(h->root);
    free(h);
}

void delete_min_stress_test() {
    heap *h = make_heap();

    // Seed RNG
    srand(time(0));

    // Insertion
    printf("Inserting...\n");
    for (int i = 0; i < (1 << 20); i++) {
        printf("Insert #%d\n", i);
        insert(h, 1 + (rand()));
        // insert(h, 5);
        //assert_that(h->size == i+1, "SIZE, MOTHERFUCKER!");
    }

    // Deletion
    printf("Deleting...\n");
    int max = -1;
    int prev_size;
    while (h->size > 0) {
        prev_size = h->size;

        node *min = delete_min(h);
        printf("Deleting key %d\n", min->key);

        assert_that(min->key >= max, "KEY OH NO LOL D:");
        max = min->key;
        assert_node_has_lowest_key(h->root);
        assert_that(h->size == prev_size - 1, "SIZE AGAIN!");
    }

    free(h);
}

void decrease_key_single_node_heap() {
    heap *h = make_heap();
    insert(h, 2);
    assert_that(h->root->key == 2, "Key must be 2");
    decrease_key(h, h->root, 1);
    assert_that(h->root->key == 1, "Key must be 1");
    free(h);
}

void decrease_key_multi_node_heap() {
    heap *h = make_heap();
    insert(h, 2);
    insert(h, 10);
    insert(h, 59);
    insert(h, 1);
    node *n = insert(h, 12);
    decrease_key(h, n, 1);
    assert_that(h->root->key == 1, "Key must still be 1");
    decrease_key(h, n, 11);
    assert_that(h->root->key == 0, "Key must be 0");
    free(h);
}

void validate(node *n) {
    if(n->parent != NULL) {
        if (n->parent->child != n) {
            node *this = n;
            int seenchild = 0;
            do {
                if (this == n->parent->child) seenchild = 1;
                this = this->right_sibling;
            } while (this != n);
            if (!seenchild) {
                printf("PARENTS CHILD-POINTER IS WRONG");
                exit(-1);
            }
        }
    }
    node *this = n;
    do {
        if (this->parent != n->parent) {
            printf("PARENTS ARE NOT EQUAL");
            exit(-1);
        }
        this = this->right_sibling;
    } while (this != n);
    if (n->right_sibling->left_sibling != n) {
        printf("RIGHT SIB IS WRONG");
        exit(-1);
    }
    if (n->left_sibling->right_sibling != n) {
        printf("LEFT SIB IS WRONG");
        exit(-1);
    }
    if (n->child != NULL) {
        if (n->child->parent != n) {
            printf("CHILD IS WRONG");
            exit(-1);
        }
    }
}


void visit_nodes(node *n, void (*fun)(node *n)) {
    if (n == NULL) return;
    node *current = n;
    do {
        (*fun)(current);
        visit_nodes(n->child, fun);
        current = current->right_sibling;
    } while (current != n);
}

void decrease_key_stress_test() {
    heap *h = make_heap();

    // Seed RNG
    srand(time(0));

    // Test size
    int n = (1 << 5);

    // Make an array to contain all the nodes
    node **nodes = (node**) malloc(n * sizeof(node*));

    // Insertion
    printf("Inserting...\n");
    for (int i = 0; i < n; i++) {
        printf("Insert #%d\n", i);
        nodes[i] = insert(h, 1 + (rand() % 100));
    }

    insert(h, -1);
    insert(h, -1);
    insert(h, -1);

    for (int i = 0; i < 3; i++) {
        printf("Size: %d\n", h->size);
        delete_min(h);
    }

    // Decrease
    //build_dot(h, "some.dot");
    printf("Decreasing...\n");
    for (int i = 0; i < (1<<20); i++) {
        int r = rand() % n;
        int amount = rand() % 20;
        //build_dot(h, "before.dot");
        //printf("Decrease %d to %d\n", nodes[r]->key, nodes[r]->key - amount);
        decrease_key(h, nodes[r], amount);
        //build_dot(h, "after.dot");
        visit_nodes(h->root, validate);
    }
    // build_dot(h, "before.dot");

    // Free Willy!
    free(nodes);
    free(h);
}

int main() {
   p("Running tests...");
   insert_in_empty_heap();
   insert_in_non_empty_heap();
   delete_min_in_single_node_heap_p1();
   delete_min_in_multi_node_heap_p1();
   delete_min_in_multi_node_heap_p2();
   delete_min_in_multi_node_heap_p3();
   delete_min_stress_test();
   decrease_key_single_node_heap();
   decrease_key_multi_node_heap();
   decrease_key_stress_test();
   p("All tests OK!");
   return 0;
}