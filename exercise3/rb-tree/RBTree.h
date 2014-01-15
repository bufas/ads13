#pragma once

#include "RBTreeNode.h"

template<typename T>
class RBTree {

	public:

		RBTreeNode<T> *root;
		int size;

		RBTree<T>() : root(nullptr), size(0) {
		}

		~RBTree<T>() {
			delete root;
			root = nullptr;
		}

		RBTreeNode<T>* insert(int key, T value);
		bool remove(int key);
		bool remove(RBTreeNode<T> *z);
		int predecessor(int x);
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

#include "RBTreeImpl.h"
