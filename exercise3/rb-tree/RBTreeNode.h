#pragma once

template<typename T>
class RBTreeNode {

	public:
		RBTreeNode<T> *p;
		RBTreeNode<T> *left;
		RBTreeNode<T> *right;
		bool red;
		int key;
        T value;

        RBTreeNode<T>(RBTreeNode<T> *p, int k, T v) : RBTreeNode<T>(p, nullptr, nullptr, k, v) {}

        RBTreeNode<T>(RBTreeNode<T> *p, RBTreeNode<T> *l, RBTreeNode<T> *r, int k, T v) 
            : p(p), left(l), right(r), red(true), key(k), value(v) {}

        ~RBTreeNode<T>() {
            delete left;  left  = nullptr;
            delete right; right = nullptr;
        }

};