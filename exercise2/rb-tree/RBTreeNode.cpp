#include <iostream>
#include "RBTreeNode.h"

RBTreeNode::RBTreeNode(RBTreeNode *p, RBTreeNode *l, RBTreeNode *r, int v, bool sentinel) 
	: p(p), left(l), right(r), red(true), value(v), sentinel(sentinel) {}

RBTreeNode::~RBTreeNode() {
	// Delete the children if they do not point to the sentinel
	if (!sentinel && !left->sentinel) delete left;
	if (!sentinel && !right->sentinel) delete right;
}
