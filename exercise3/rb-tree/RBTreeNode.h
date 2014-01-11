#pragma once

#include "RBTree.h"

class RBTreeNode {

	public:
		RBTreeNode *p;
		RBTreeNode *left;
		RBTreeNode *right;
		bool red;
		int value;
		bool sentinel;

		RBTreeNode(RBTreeNode *p, RBTreeNode *l, RBTreeNode *r, int v, bool sentinel = false);
		~RBTreeNode();

};
