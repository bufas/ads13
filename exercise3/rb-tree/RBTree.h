#pragma once

#include "RBTreeNode.h"

class RBTreeNode;
class RBTree {

	public:

		RBTreeNode *root;
		RBTreeNode *sentinel;

		RBTree();
		~RBTree();

		RBTreeNode* insert(int value);
		bool remove(int key);
		bool remove(RBTreeNode *z);
		RBTreeNode* predecessor(RBTreeNode *z);
		void remove_min();
		RBTreeNode* find_min();
		RBTreeNode* get(int key);

		void print();
		bool validate();

//	private:

		void insert_fixup(RBTreeNode *z);
		void remove_fixup(RBTreeNode *x);
		void transplant(RBTreeNode *u, RBTreeNode *v);

		void rotate_left(RBTreeNode *x);
		void rotate_right(RBTreeNode *x);

};
