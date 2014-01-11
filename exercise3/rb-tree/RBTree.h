#pragma once

#include "RBTreeNode.h"

template<typename T>
class RBTree {

	public:

		RBTreeNode<T> *root;
		RBTreeNode<T> *sentinel;

		RBTree<T>() : root(nullptr) {
			sentinel = new RBTreeNode<T>(nullptr, nullptr, nullptr, 0, true);
			sentinel->red = false;
		}

		~RBTree<T>() {
			delete root;
			delete sentinel;
		}

		RBTreeNode<T>* insert(int key, T value);
		bool remove(int key);
		bool remove(RBTreeNode<T> *z);
		RBTreeNode<T>* predecessor(RBTreeNode<T> *z);
		void remove_min();
		RBTreeNode<T>* find_min();
		RBTreeNode<T>* get(int key);

		void print();
		bool validate();

//	private:

		void insert_fixup(RBTreeNode<T> *z);
		void remove_fixup(RBTreeNode<T> *x);
		void transplant(RBTreeNode<T> *u, RBTreeNode<T> *v);

		void rotate_left(RBTreeNode<T> *x);
		void rotate_right(RBTreeNode<T> *x);

};
