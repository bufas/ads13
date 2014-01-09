#ifndef BIN_HEAP_CPP_
#define BIN_HEAP_CPP_

#include <iostream>
#include "heap.hpp"
#include "node.hpp"

using std::cout;
using std::endl;

/**
 * Forward declarations
 */
template<typename T> class BinHeap;
template<typename T> class BinNode;

/**
 * Internal node used by the binary heap implementation.
 */
template<typename T>
class BinNode : public Node<T> {
    public:
        BinNode(const int key, const T value) : key_(key), value_(value), leftChild_(NULL), rightChild_(NULL), parent_(NULL) {
        }

        ~BinNode() {
            delete leftChild_;
            delete rightChild_;
        }

        int getKey() {
            return key_;
        }

        T getValue() {
            return value_;
        }

        friend class BinHeap<T>;

    private:
        int key_;
        const T value_;

        BinNode<T> *parent_;
        BinNode<T> *leftChild_;
        BinNode<T> *rightChild_;
};

/**
 * Binary heap declaration
 */
template<typename T>
class BinHeap : public Heap<T> {
    public:
        BinHeap() : size_(0) {
        }

        ~BinHeap() {
            delete root_;
        }

        Node<T>* insert(int key, T value);
        Node<T>* findMin();
        Node<T>* deleteMin();
        void decreaseKey(Node<T> *n, int delta);
        int size();

    private:
        int size_;
        BinNode<T> *root_;

        void bubbleUp(BinNode<T> *n);
        void rotateUp(BinNode<T> *n);
        void siftDown(BinNode<T> *n);
};

#define CHECK_BIT(s,b) (s >> b) & 1
/**
 * Insert implementation
 */
/* TODO: MAKE THIS WORK!
template<typename T>
Node<T> BinHeap<T>::insert(int key, T value) {
    BinNode<T> *n = new BinNode<T>(key, value);
    if (root_ == NULL) {
        root_ = n;
    } else {
        // Locate the first 1 in the bit string of size_
        int i = 30;
        for (; i >= 0; i--) {
            if (CHECK_BIT(size_+1, i)) break;
        }

        // Then look for the first available child pointer
        BinNode<T> *parent = root_;
        for (i--; i >= 0; i--) {
            // 0 -> go left, 1 -> go right
//             BinNode<T> **child = !CHECK_BIT(size_+1, i)
//                 ? &(parent->leftChild_)
//                 : &(parent->rightChild_);

//             if (*child == NULL) {
//                 n->parent_ = parent;
//                 *child = n;
//                 break;
//             } else {
//                 parent = *child;
//             }

            if (!CHECK_BIT(size_+1, i)) {
                if (parent->leftChild_ == NULL) {
                    n->parent_ = parent;
                    parent->leftChild_ = n;
                    break;
                } else {
                    parent = parent->leftChild_;
                }
            } else {
                if (parent->rightChild_ == NULL) {
                    n->parent_ = parent;
                    parent->rightChild_ = n;
                    break;
                } else {
                    parent = parent->rightChild_;
                }
            }
        }

        // Finally, bubble the node up in the heap
        bubbleUp(n);
    }
    size_++;
    return *n;
}
*/

template<typename T>
Node<T>* BinHeap<T>::insert(int key, T value) {
    BinNode<T> *n = new BinNode<T>(key, value);
    if (root_ == NULL) {
        root_ = n;
    } else {
        bool seenOne = false;
        BinNode<T> *parent = root_;
        for (int i = 30; i >= 0; i--) {
            int bitN = CHECK_BIT(size_+1, i);

            if (seenOne && !bitN) {
                if (parent->leftChild_ == NULL) {
                    n->parent_ = parent;
                    parent->leftChild_ = n;
                    break;
                } else {
                    parent = parent->leftChild_;
                }
            }

            if (seenOne && bitN) {
                if (parent->rightChild_ == NULL) {
                    n->parent_ = parent;
                    parent->rightChild_ = n;
                    break;
                } else {
                    parent = parent->rightChild_;
                }
            }

            if (!seenOne && bitN) seenOne = true;
        }
        bubbleUp(n);
    }

    size_++;
    return n;
}

template<typename T>
Node<T>* BinHeap<T>::findMin() {
    return root_;
}

template<typename T>
Node<T>* BinHeap<T>::deleteMin() {
    BinNode<T> *min = root_;
    if (size_ == 1) {
        root_ = NULL;
        size_ = 0;
        return min;
    } else if (size_ == 2) {
        root_ = root_->leftChild_;
        root_->parent_ = NULL;
        size_ = 1;
        return min;
    }

    // Find the last node
    bool seenOne = false;
    BinNode<T> *last = root_;
    int bitN;
    for (int i = 30; i >= 0; i -= 1) {
        bitN = CHECK_BIT(size_, i);

        // Go left or right depending on the bit value
        if (seenOne && !bitN) last = last->leftChild_;
        if (seenOne && bitN)  last = last->rightChild_;

        if (!seenOne && bitN) seenOne = true;
    }

    // Set the root's children's parent pointers to last
    root_->leftChild_->parent_  = last;
    root_->rightChild_->parent_ = last;

    // Set the last node's children pointers to the root's children
    last->leftChild_  = root_->leftChild_;
    last->rightChild_ = root_->rightChild_;

    // Null the last node's parent's child pointer
    if (bitN) last->parent_->rightChild_ = NULL;
    else      last->parent_->leftChild_ = NULL;

    // Null the last node's parent pointer
    last->parent_ = NULL;

    // Null the root's child pointers
    root_->leftChild_  = NULL;
    root_->rightChild_ = NULL;

    // Update the heap size
    size_--;

    // Restore heap order
    siftDown(last);

    // Find the new root
    while (last->parent_ != NULL) {
        last = last->parent_;
    }

    // Delete the old root and replace it with the new
    //delete root_;
    root_ = last;
    return min;
}

template<typename T>
int BinHeap<T>::size() {
    return size_;
}

template<typename T>
void BinHeap<T>::decreaseKey(Node<T> *n, int delta) {
    BinNode<T> *bn = (BinNode<T> *) n;
    bn->key_ -= delta;
    bubbleUp(bn);
}

/**
 * Auxiliary function for bubbling nodes up
 */
template<typename T>
void BinHeap<T>::bubbleUp(BinNode<T> *n) {
    while (n->parent_ != NULL && n->parent_->key_ > n->key_) {
        rotateUp(n);
    }

    if (n->parent_ == NULL) {
        root_ = n;
    }
}

/**
 * Auxiliary function for rotating a node upwards, making it switch
 * places with its parent, thus swapping all their pointers.
 */
template<typename T>
void BinHeap<T>::rotateUp(BinNode<T> *n) {
    // Set variables
    BinNode<T> *parent = n->parent_;
    BinNode<T> *leftChild_ = n->leftChild_;
    BinNode<T> *rightChild_ = n->rightChild_;
    BinNode<T> *grandparent = NULL;
    if(parent != NULL) grandparent = n->parent_->parent_;

    // Swap n with its parent, there are 10 pointers to update
    if(leftChild_ != NULL) leftChild_->parent_ = parent;   // 1
    if(rightChild_ != NULL) rightChild_->parent_ = parent; // 2

    // Find out if n is the right or left child
    BinNode<T> *sibling = NULL;
    if (n == parent->leftChild_) {
        // n is the left child
        sibling = parent->rightChild_;
        n->leftChild_ = parent;                           // 4
        n->rightChild_ = sibling;                         // 5
    } else {
        // n is the right child
        sibling = parent->leftChild_;
        n->leftChild_ = sibling;                          // 4
        n->rightChild_ = parent;                          // 5
    }

    if (sibling != NULL) sibling->parent_ = n;             // 3

    // Find out if parent is right or left child
    if (grandparent != NULL) { 
        if (parent == grandparent->leftChild_) {
            // parent is left child
            grandparent->leftChild_ = n;                  // 6
        } else {
            // parent is right child
            grandparent->rightChild_ = n;                 // 6
        }
    }

    parent->leftChild_ = leftChild_;                      // 7
    parent->rightChild_ = rightChild_;                    // 8

    parent->parent_ = n;                                   // 9
    n->parent_ = grandparent;                              // 10
}

/**
 * Auxiliary function for sifting nodes down
 */
template<typename T>
void BinHeap<T>::siftDown(BinNode<T> *n) {
    // Return if there is no children (if left child is NULL then right child is NULL)
    if (n->leftChild_ == NULL) return;

    if (n->key_ <= n->leftChild_->key_) {
        // Left child's key is greater than n's key
        if (n->rightChild_ == NULL) return;
        if (n->rightChild_->key_ >= n->key_) return;

        // Switch with right child
        rotateUp(n->rightChild_);
    } else {
        // Left key is less than n's key
        if (n->rightChild_ == NULL) {
            // Switch with left child
            rotateUp(n->leftChild_);
        } else {
            if (n->rightChild_->key_ > n->leftChild_->key_) {
                // Switch with left child
                rotateUp(n->leftChild_);
            } else {
                // Switch with right child
                rotateUp(n->rightChild_);
            }
        }
    }
    siftDown(n);
}

#endif