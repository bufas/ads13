#pragma once

template<typename T>
class RBTreeNode {

	public:
		RBTreeNode<T> *p;
		RBTreeNode<T> *left;
		RBTreeNode<T> *right;
		bool red;
		int key;
		bool sentinel;
        T value;

        RBTreeNode<T>(RBTreeNode<T> *p, RBTreeNode<T> *l, RBTreeNode<T> *r, int k, bool sentinel, T v) 
            : p(p), left(l), right(r), red(true), key(k), sentinel(sentinel), value(v) {}

        ~RBTreeNode<T>() {
            // Delete the children if they do not point to the sentinel
            if (!sentinel && left  != nullptr) delete left;
            if (!sentinel && right != nullptr) delete right;
        }

};