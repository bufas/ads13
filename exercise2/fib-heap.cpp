#ifndef FIB_HEAP_CPP_
#define FIB_HEAP_CPP_

#include <iostream>
#include <cmath>
#include "heap.hpp"
#include "node.hpp"

using std::cout;
using std::endl;

/**
 * Forward declarations
 */
template<typename T> class FibHeap;
template<typename T> class FibNode;

/**
 * Internal node used by the Fibonacci heap implementation.
 */
template<typename T>
class FibNode : public Node<T> {
    public:
        FibNode(const int key, const T value) : key_(key), value_(value), rank_(0), marked_(false), child_(NULL), leftSibling_(this), rightSibling_(this), parent_(NULL) {
        }

        ~FibNode() {
            delete child_;
        }

        int getKey() {
            return key_;
        }

        T getValue() {
            return value_;
        }

        friend class FibHeap<T>;

    private:
        int key_;
        int rank_;
        bool marked_;
        const T value_;

        FibNode<T> *parent_;
        FibNode<T> *child_;
        FibNode<T> *leftSibling_;
        FibNode<T> *rightSibling_;

        bool hasSiblings();
        bool hasChildren();
        int siblingCount();

        void meld(FibNode<T> *n2);
        void cutFromSiblingList();
        void cutSubtree();
};

/**
 * Fibonacci heap declaration
 */
template<typename T>
class FibHeap : public Heap<T> {
    public:
        FibHeap() : size_(0), root_(NULL) {
        }

        ~FibHeap() {
            delete root_;
        }

        Node<T>* insert(int key, T value);
        Node<T>* findMin();
        Node<T>* deleteMin();
        void decreaseKey(Node<T> *n, int delta);
        int size();

    private:
        int size_;
        FibNode<T> *root_;

        int rootCount();
        void deleteMinPhase1();
        void deleteMinPhase2();
        void deleteMinPhase3();
        void decreaseKeyCut(FibNode<T> *n);
};

//=============================================================================
//  Auxiliary functions
//=============================================================================

template<typename T>
bool FibNode<T>::hasSiblings() {
    return rightSibling_ != this;
}

template<typename T>
bool FibNode<T>::hasChildren() {
    return child_ != NULL;
}

template<typename T>
int FibNode<T>::siblingCount() {
    int count = 0;
    FibNode<T> *current = this;
    do {
        count++;
        current = current->rightSibling_;
    } while (current != this);
    return count;
}

template<typename T>
int FibHeap<T>::rootCount() {
    return root_->siblingCount();
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
template<typename T>
void FibNode<T>::meld(/*FibNode<T> *n1, */FibNode<T> *n2) {
    FibNode<T> *n1 = this;

    // Grab r1 and l2
    FibNode<T> *r1 = n1->rightSibling_;
    FibNode<T> *l2 = n2->leftSibling_;

    // Update pointers: n1->n2 and n1<-n2
    n1->rightSibling_ = n2;
    n2->leftSibling_ = n1;

    // Update pointers: l2<-r1 and l2->r1
    r1->leftSibling_ = l2;
    l2->rightSibling_ = r1;
}

// Cut a node out of its sibling list by making its closest siblings point
// to each other instead of the given node. Both sibling pointers of the
// node are updated to point at the node itself.
template<typename T>
void FibNode<T>::cutFromSiblingList(/*node *n*/) {
    FibNode<T> *rsib = rightSibling_;
    FibNode<T> *lsib = leftSibling_;

    // Update pointers:  l<-r and l->r
    rsib->leftSibling_ = lsib;
    lsib->rightSibling_ = rsib;

    // Update pointers:  n<-n and n->n
    leftSibling_ = this;
    rightSibling_ = this;
}

template<typename T>
void FibNode<T>::cutSubtree(/*node *n*/) {
    // Remove from parent
    if (parent_ != NULL) {
        if (parent_->child_ == this) {
            if (hasSiblings()) {
                parent_->child_ = rightSibling_;
            } else {
                parent_->child_ = NULL;
            }
        }

        parent_->rank_ -= 1;
        parent_ = NULL;
    }

    // Cut from sibling list
    cutFromSiblingList();
}



//=============================================================================
//  DeleteMin phases
//=============================================================================

template<typename T>
void FibHeap<T>::deleteMinPhase1(/*heap *h*/) {
    FibNode<T> *min = root_;
    if (min->hasChildren()) {
        // Orphan all children
        FibNode<T> *first = min->child_;
        FibNode<T> *curr  = min->child_;
        do {
            curr->parent_ = NULL;
            curr = curr->rightSibling_;
        } while (curr != first);

        // If the root has siblings, first cut it out of its sibling list
        // and then meld sibling and children lists
        if (min->hasSiblings()) {
            FibNode<T> *rsib = min->rightSibling_;
            min->cutFromSiblingList();
            first->meld(rsib);
        }

        // Pick the first child as the new temporary root
        root_ = first;
    } else {
        // Due to the early return in the top, we are guaranteed that
        // if min has no children, it must have siblings, so we start
        // by cutting it out of its sibling list.
        FibNode<T> *sib = min->rightSibling_;
        min->cutFromSiblingList();

        // Pick a sibling (arbitrary) as the new temporary root
        root_ = sib;
    }
}

template<typename T>
void FibHeap<T>::deleteMinPhase2(/*heap *h*/) {
    int max_rank = (int) ceil(2 * (log(size_ + 1) / log(2.0)));
    FibNode<T> *ranks[max_rank];
    for (int i = 0; i < max_rank; i++) {
        ranks[i] = NULL;
    }

    // The chain of root nodes is modified in the linking step, so to make
    // absolutely sure we go through all the root nodes, we store pointers
    // to them in an array, and iterate the array instead of depending on
    // the possibly shifty sibling list.
    int n = rootCount();
    FibNode<T> **roots = (FibNode<T>**) malloc(n * sizeof(FibNode<T>*));

    // Populate the array
    FibNode<T> *current = root_;
    for (int i = 0; i < n; i++) {
        roots[i] = current;
        current = current->rightSibling_;
    }

    // Go through each node in the array
    for (int i = 0; i < n; i++) {
        FibNode<T> *curr = roots[i];

        // If there are pre-existing array entries, start linking
        while (ranks[curr->rank_] != NULL) {
            // Determine winner and loser
            FibNode<T> *winner = (curr->key_ < ranks[curr->rank_]->key_ ? curr : ranks[curr->rank_]);
            FibNode<T> *loser  = (curr->key_ < ranks[curr->rank_]->key_ ? ranks[curr->rank_] : curr);

            // Null the array entry right away
            ranks[curr->rank_] = NULL;

            // Update loser's parent pointer and promote winner
            loser->parent_ = winner;
            winner->rank_ += 1;

            // Cut loser out of its sibling list, and meld with the
            // children list of the winner (if any, otherwise just
            // set the loser as the only child of the winner).
            loser->cutFromSiblingList();
            if (winner->hasChildren()) {
                winner->child_->meld(loser);
            } else {
                winner->child_ = loser;
            }

            // Update current to be the winner
            curr = winner;
        }

        // Otherwise/finally, insert current into its place in the
        // array, and make it the temporary heap root.
        ranks[curr->rank_] = curr;
        root_ = curr;
    }

    // Free the possibly HUGE array
    free(roots);
}

template<typename T>
void FibHeap<T>::deleteMinPhase3(/*heap *h*/) {
    FibNode<T> *first = root_;
    FibNode<T> *curr  = root_;
    do {
        if (curr->key_ < root_->key_) {
            root_ = curr;
        }
        curr = curr->rightSibling_;
    } while (curr != first);
}

template<typename T>
Node<T>* FibHeap<T>::insert(int key, T value) {
    FibNode<T> *n = new FibNode<T>(key, value);

    // Meld it with the root of the heap, if it is non-null
    if (root_ != NULL) {
        root_->meld(n);
        if (key < root_->key_) {
            root_ = n;
        }
    } else {
        root_ = n;
    }

    // Remember to update the size of the heap
    size_ += 1;
    return n;
}

template<typename T>
Node<T>* FibHeap<T>::findMin() {
    return root_;
}

template<typename T>
Node<T>* FibHeap<T>::deleteMin(/*heap *h*/) {
    // In the special case where the heap root has no siblings and no
    // children, we can simply null the root, decrease the size, and
    // return early, as no complicated work is required.
    FibNode<T> *min = root_;
    if (!min->hasChildren() && !min->hasSiblings()) {
        root_ = NULL;
        size_ -= 1;
        return min;
    }

    // Step 1: Remove minimum node (min)
    // If min has children, they are orphaned. If min also has siblings,
    // min is cut out from the sibling list, which is then combined with
    // the children list.
    // If min has siblings but no children, it is just cut out from the
    // sibling list.
    deleteMinPhase1();

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
    deleteMinPhase2();

    // Step 3: Find new min
    // We simply iterate over all root nodes, updating the root pointer
    // of the heap every time we find a smaller key
    deleteMinPhase3();

    // Finally, decrease heap size, and return the extracted min node
    size_ -= 1;
    return min;
}

template<typename T>
int FibHeap<T>::size() {
    return size_;
}

template<typename T>
void FibHeap<T>::decreaseKeyCut(/*heap *h, node *n*/ FibNode<T> *n) {
    // Cut is only relevant on non-roots
    if (n->parent_ != NULL) {
        // Remember the parent
        FibNode<T> *parent = n->parent_;

        // Then cut out the subtree, and insert as a new root
        n->cutSubtree();
        root_->meld(n);

        // If the parent was marked, cut recursively
        if (parent->marked_) {
            parent->marked_ = false;
            decreaseKeyCut(parent);
        } else {
            parent->marked_ = true;
        }
    }

    // Update the heap minimum
    root_ = (n->key_ < root_->key_) ? n : root_;
}

template<typename T>
void FibHeap<T>::decreaseKey(/*heap *h, node *n,*/ Node<T> *n, int delta) {
    FibNode<T> *fn = (FibNode<T> *) n;

    // Decrease the five dollar ass key, before we make change
    fn->key_ -= delta;

    // If the node is a root or heap order is preserved -> bail
    //if (n->parent == NULL || n->parent->key <= n->key) return;

    // Update the heap pointer
    decreaseKeyCut(fn);
}

#endif