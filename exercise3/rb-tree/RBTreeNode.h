#pragma once

template<typename T>
class RBTreeNode {

	public:
		RBTreeNode<T> *p;
		RBTreeNode<T> *left;
		RBTreeNode<T> *right;
		bool red;
		int value;
		bool sentinel;

        RBTreeNode<T>(RBTreeNode<T> *p, RBTreeNode<T> *l, RBTreeNode<T> *r, int v, bool sentinel) 
            : p(p), left(l), right(r), red(true), value(v), sentinel(sentinel) {}

        ~RBTreeNode<T>() {
            // Delete the children if they do not point to the sentinel
            if (!sentinel && !left->sentinel) delete left;
            if (!sentinel && !right->sentinel) delete right;
        }

};
