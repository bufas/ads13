#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fib-heap.h"

node *meld(node *, node *);

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


void print_heap(heap *h);
void print_node(node *n);
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

int has_siblings(node *n) {
    return n->right_sibling != n;
}
int has_children(node *n) {
    return n->child != NULL;
}
int sibling_count(node *n) {
    node *c = n->right_sibling;
    int count = 1;
    while (c != n) {
        count++;
        c = c->right_sibling;
    }
    return count;
}

void take_out(node *n) {
    // Handle parent first
    if (n->parent != NULL && n->parent->child == n) {
        if (has_siblings(n)) {
            n->parent->child = n->right_sibling;
        } else {
            n->parent->child = NULL;
        }
        n->parent->rank -= 1;
    }
    n->parent = NULL;

    // Handle siblings
    n->right_sibling->left_sibling = n->left_sibling;
    n->left_sibling->right_sibling = n->right_sibling;

    n->right_sibling = n;
    n->left_sibling  = n;
}


node *link(node *n1, node *n2) {
    visit_nodes(n1, validate);
    if (n1 == n2) printf("TOO MUCH MAN IST TOO MUCH\n");
    if (n1->parent != NULL || n2->parent != NULL) { printf("SOMETHING\n");exit(-1);}
    // Swap the parameters so n1 has the smallest key
    if (n2->key < n1->key) {
        return link(n2, n1);
    }

    // Remove n2 from the root list
    take_out(n2);

    n2->parent = n1;
    meld(n1->child, n2);
    n1->child = n2;
    n1->rank += 1;

    visit_nodes(n1, validate);
    return n1;
}

// TODO meld two heaps instead of two nodes
node *meld(node *n1, node *n2) {
    // This only happens when inserting first element
    if (n1 == NULL) return n2;
    if (n2 == NULL) return n1;

    n1->right_sibling->left_sibling = n2->left_sibling;
    n2->left_sibling->right_sibling = n1->right_sibling;

    n1->right_sibling = n2;
    n2->left_sibling = n1;

    return n1->key <= n2->key ? n1 : n2;
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

int some_root_has_parent(node *n) {
    int sib_ount = sibling_count(n);
    for (int i = 0; i < sib_ount; i++) {
        if (n->parent != NULL) return 1;
        n = n->right_sibling;
    }
    return 0;
}

node *delete_min(heap *h) {
    const coount = 0;
    if(coount < 2) ;
    visit_nodes(h->root, validate);
    //static int runs = 1;
    //printf("Deletes : %d\n", runs++);
    node *min = h->root;

    if(some_root_has_parent(h->root)) printf("\t\t\t 1\n");

    // print all root nodes
    node *rootx = h->root;
    int sib_ount = sibling_count(rootx);
    for (int i = 0; i < sib_ount; i++) {
        if (rootx->parent != NULL) {
            printf("A root has parent = %d\n", rootx->parent->key);
            exit(-1);
        }
        rootx = rootx->right_sibling;
    }
    //printf("\n");

    if(some_root_has_parent(h->root)) printf("\t\t\t 2\n");

    // Check the special case that root has no children and no siblings
    if (!has_children(min) && !has_siblings(min)) {
        h->size = 0;
        h->root = NULL;
        return min;
    }

    if(some_root_has_parent(h->root)) printf("\t\t\t 3\n");

    node *anchor;
    if (has_children(min)) {
        // Null child parents (rootify)
        node *child = min->child;
        int sib_count = sibling_count(child);
        for (int i = 0; i < sib_count; i++) {
            child->parent = NULL;
            child = child->right_sibling;
        }

        if(some_root_has_parent(min)) printf("\t\t\t 4\n");

        anchor = min->child;

        // Insert children into root chain (forrestify)
        if (has_siblings(min)) {
            node *r_sib = min->right_sibling;
            take_out(min);
            if(some_root_has_parent(anchor)) printf("\t\t\t 5\n");
            meld(r_sib, min->child);
            if(some_root_has_parent(anchor)) printf("\t\t\t 6\n");
        }
    } else {
        // Guaranteed siblings
        anchor = min->right_sibling;
        take_out(min);
        if(some_root_has_parent(anchor)) printf("\t\t\t 7\n");
    }
    
    if(some_root_has_parent(anchor)) printf("\t\t\t 8\n");
    // LINKING STEP
    // Create an array with max_rank entries and initialize to NULL
    int max_rank = 20;//h->size; // TODO set a tighter bound
    node *seen_ranks[max_rank];
    for (int i = 0; i < max_rank; seen_ranks[i++] = NULL);

    // Count the root nodes
    int no_roots = sibling_count(anchor);

    // Run through all roots and link equal ranked trees
    node *current = anchor;
    node *new_min = anchor;
    if(some_root_has_parent(anchor)) printf("\t\t\t 9\n");
    for (int i = 0; i < no_roots; i++) {
        if(some_root_has_parent(new_min)) printf("\t\t\t for-begin %d\n", i);
        // Remember the next node
        node *next = current->right_sibling;

        int cur_rank = current->rank;
        while (seen_ranks[cur_rank] != NULL) {
            if(some_root_has_parent(new_min)) printf("\t\t\t while-begin %d\n", current->key);
            if(some_root_has_parent(current)) printf("\t\t\t while-begin2 %d\n", current->key);
            current = link(seen_ranks[cur_rank], current);

            if(some_root_has_parent(new_min) && current->key >= new_min->key) {
                printf("%d\n", new_min->parent->key);
                visit_nodes(new_min, validate);
                if(seen_ranks[cur_rank] == new_min) printf("WASA");
                printf("\t\t\t while-end %d count %d\n", current->key, sibling_count(new_min));
            }
            seen_ranks[cur_rank] = NULL;
            cur_rank = current->rank;
            if(some_root_has_parent(current)) printf("\t\t\t while-end2 %d\n", current->key);
        }
        if(some_root_has_parent(new_min) && current->key >= new_min->key) printf("\t\t\t for-mid %d\n", i);

        // Update new min if current has a smaller key
        if (current->key < new_min->key) {
            new_min = current;
        }

        // Insert the current node into the array and proceed to next node
        seen_ranks[current->rank] = current;
        current = next;
        if(some_root_has_parent(new_min)) printf("\t\t\t for-end %d\n", i);
    }
    if(some_root_has_parent(new_min)) printf("\t\t\t 10\n");

    // Update the heap
    h->root = new_min;
    h->size -= 1;
    if(some_root_has_parent(new_min)) printf("\t\t\t 11\n");

    // Return the deleted node
    visit_nodes(new_min, validate);
    coount++;
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
