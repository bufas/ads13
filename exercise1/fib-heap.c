#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "fib-heap.h"
#include "fib-dot.h"

//=============================================================================
//  Auxiliary functions
//=============================================================================

int has_siblings(node *n) {
    return n->right_sibling != n;
}

int has_children(node *n) {
    return n->child != NULL;
}

// Number of siblings in the sibling list of n, including n itself, i.e. the
// size of the doubly-linked list that through n and its siblings.
int sibling_count(node *n) {
    int count = 0;
    node *current = n;
    do {
        count++;
        current = current->right_sibling;
    } while (current != n);
    return count;
}

// Number of root trees in a heap, i.e. the sibling count of min.
int root_count(heap *h) {
    return sibling_count(h->root);
}

// Combine the doubly linked sibling lists of the given nodes by taking n1
// to be the right-most child in its list, and n2 to be the left-most in its
// list:
//
//      List 1: *-r1-...-n1-*
//      List 2: *-n2-...-l2-*
//
// n2 then becomes the right sibling of n1, and the previous right sibling
// becomes the right sibling of the right-most sibling in the list of n2, and
// vice versa for n1:
//
//      New List: *-r1-...-n1-n2-...-l2-*
//
// It is assumed that all nodes have the same parents.
void meld(node *n1, node *n2) {
    // Grab r1 and l2
    node *r1 = n1->right_sibling;
    node *l2 = n2->left_sibling;

    // Update pointers: n1->n2 and n1<-n2
    n1->right_sibling = n2;
    n2->left_sibling = n1;

    // Update pointers: l2<-r1 and l2->r1
    r1->left_sibling = l2;
    l2->right_sibling = r1;
}

// Cut a node out of its sibling list by making its closest siblings point
// to each other instead of the given node. Both sibling pointers of the
// node are updated to point at the node itself.
void cut_from_sibling_list(node *n) {
    node *rsib = n->right_sibling;
    node *lsib = n->left_sibling;

    // Update pointers:  l<-r and l->r
    rsib->left_sibling = lsib;
    lsib->right_sibling = rsib;

    // Update pointers:  n<-n and n->n
    n->left_sibling = n;
    n->right_sibling = n;
}

void cut_subtree(node *n) {
    // Remove from parent
    if (n->parent != NULL) {
        if (n->parent->child == n) {
            if (has_siblings(n)) {
                n->parent->child = n->right_sibling;
            } else {
                n->parent->child = NULL;
            }
        }

        n->parent->rank -= 1;
        n->parent = NULL;
    }

    // Cut from sibling list
    cut_from_sibling_list(n);
}

void delete_min_phase_1(heap *h) {
    node *min = h->root;
    if (has_children(min)) {
        // Orphan all children
        node *first = min->child;
        node *curr  = min->child;
        do {
            curr->parent = NULL;
            curr = curr->right_sibling;
        } while (curr != first);

        // If the root has siblings, first cut it out of its sibling list
        // and then meld sibling and children lists
        if (has_siblings(min)) {
            node *rsib = min->right_sibling;
            cut_from_sibling_list(min);
            meld(first, rsib);
        }

        // Pick the first child as the new temporary root
        h->root = first;
    } else {
        // Due to the early return in the top, we are guaranteed that
        // if min has no children, it must have siblings, so we start
        // by cutting it out of its sibling list.
        node *sib = min->right_sibling;
        cut_from_sibling_list(min);

        // Pick a sibling (arbitrary) as the new temporary root
        h->root = sib;
    }
}

void delete_min_phase_2(heap *h) {
    int max_rank = (int) ceil(2 * (log(h->size + 1) / log(2.0)));
    node *ranks[max_rank];
    for (int i = 0; i < max_rank; i++) {
        ranks[i] = NULL;
    }

    // The chain of root nodes is modified in the linking step, so to make
    // absolutely sure we go through all the root nodes, we store pointers
    // to them in an array, and iterate the array instead of depending on
    // the possibly shifty sibling list.
    int n = root_count(h);
    node **roots = (node**) malloc(n * sizeof(node*));

    // Populate the array
    node *current = h->root;
    for (int i = 0; i < n; i++) {
        roots[i] = current;
        current = current->right_sibling;
    }

    // Go through each node in the array
    for (int i = 0; i < n; i++) {
        node *curr = roots[i];

        // If there are pre-existing array entries, start linking
        while (ranks[curr->rank] != NULL) {
            // Determine winner and loser
            node *winner = (curr->key < ranks[curr->rank]->key ? curr : ranks[curr->rank]);
            node *loser  = (curr->key < ranks[curr->rank]->key ? ranks[curr->rank] : curr);

            // Null the array entry right away
            ranks[curr->rank] = NULL;

            // Update loser's parent pointer and promote winner
            loser->parent = winner;
            winner->rank += 1;

            // Cut loser out of its sibling list, and meld with the
            // children list of the winner (if any, otherwise just
            // set the loser as the only child of the winner).
            cut_from_sibling_list(loser);
            if (has_children(winner)) {
                meld(winner->child, loser);
            } else {
                winner->child = loser;
            }

            // Update current to be the winner
            curr = winner;
        }

        // Otherwise/finally, insert current into its place in the
        // array, and make it the temporary heap root.
        ranks[curr->rank] = curr;
        h->root = curr;
    }

    // Free the possibly HUGE array
    free(roots);
}

void delete_min_phase_3(heap *h) {
    node *first = h->root;
    node *curr  = h->root;
    do {
        if (curr->key < h->root->key) {
            h->root = curr;
        }
        curr = curr->right_sibling;
    } while (curr != first);
}

//=============================================================================
//  Heap functions
//=============================================================================

heap* make_heap() {
    heap *h = (heap*) malloc(sizeof(heap));
    h->root = NULL;
    h->size = 0;
    return h;
}

node find_min(heap *h) {
    return *(h->root);
}

node* insert(heap *h, int key) {
    // Create a brand new node
    node *n = (node*) malloc(sizeof(node));
    n->parent = NULL;
    n->child  = NULL;
    n->left_sibling  = n;
    n->right_sibling = n;
    n->key = key;
    n->rank = 0;
    n->marked = 0;
    n->data = NULL;

    // Meld it with the root of the heap, if it is non-null
    if (h->root != NULL) {
        meld(h->root, n);
        if (key < h->root->key) {
            h->root = n;
        }
    } else {
        h->root = n;
    }

    // Remember to update the size of the heap
    h->size++;
    return n;
}

node* delete_min(heap *h) {
    // In the special case where the heap root has no siblings and no
    // children, we can simply null the root, decrease the size, and
    // return early, as no complicated work is required.
    node *min = h->root;
    if (!has_children(min) && !has_siblings(min)) {
        h->root = NULL;
        h->size -= 1;
        return min;
    }

    // Step 1: Remove minimum node (min)
    // If min has children, they are orphaned. If min also has siblings,
    // min is cut out from the sibling list, which is then combined with
    // the children list.
    // If min has siblings but no children, it is just cut out from the
    // sibling list.
    delete_min_phase_1(h);

    // Step 2: Link roots of same rank/degree
    // We use an array to keep track of the last seen root of each rank,
    // and link roots whenever we get a collision. The node that remains
    // a root gets a promotion (rank++), and is attempted inserted into
    // its new place in the array, where we again link in case of a new
    // collision, and so on.
    // To make sure the root pointer in the heap continues to point to
    // an actual root, we update it to point to the "link winner" every
    // time we link.
    // Because the initial root may not remain as such, we also have to
    // keep track of an "end" node, i.e. the right-most (last) node in
    // the sibling list of the root (its left sibling), starting from
    // the initial root itself.
    // In the linking step, the "loser" (the root with the highest key
    // value) is cut out of its sibling list, and combined with the
    // children list of the "winner".
    delete_min_phase_2(h);

    // Step 3: Find new min
    // We simply iterate over all root nodes, updating the root pointer
    // of the heap every time we find a smaller key
    delete_min_phase_3(h);

    // Finally, decrease heap size, and return the extracted min node
    h->size -= 1;
    return min;
}

void decrease_key_cut(heap *h, node *n) {
    // Cut is only relevant on non-roots
    if (n->parent != NULL) {
        // Remember the parent
        node *parent = n->parent;

        // Then cut out the subtree, and insert as a new root
        cut_subtree(n);
        meld(h->root, n);

        // If the parent was marked, cut recursively
        if (parent->marked) {
            parent->marked = 0;
            decrease_key_cut(h, parent);
        } else {
            parent->marked = 1;
        }
    }

    // Update the heap minimum
    h->root = (n->key < h->root->key) ? n : h->root;
}

void decrease_key(heap *h, node *n, int delta) {
    // Decrease the five dollar ass key, before we make change
    n->key -= delta;

    // If the node is a root or heap order is preserved -> bail
    //if (n->parent == NULL || n->parent->key <= n->key) return;

    // Update the heap pointer
    decrease_key_cut(h, n);
}