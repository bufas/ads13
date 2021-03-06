
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "RBTree.h"

using namespace std;

RBTree::RBTree() : root(nullptr) {
	sentinel = new RBTreeNode(nullptr, nullptr, nullptr, 0, true);
	sentinel->red = false;
	// TODO should the sentinel be its own parent?
	// TODO should the sentinel be its own children?
}

RBTree::~RBTree() {
	delete root;
	delete sentinel;
}

RBTreeNode* RBTree::get(int key) {
	RBTreeNode *x = root;
	while (x->value != key && x != sentinel) {
		x = (key > x->value) ? x->right : x->left;
	}
	return x;
}

RBTreeNode* RBTree::insert(int value) {
	RBTreeNode *y = sentinel;
	RBTreeNode *x = (root == nullptr) ? sentinel : root; // Special case when tree is empty

	while (x != sentinel) {
		y = x;
		if (value < x->value) {
			x = x->left;
		} else {
			x = x->right;
		}
	}

	RBTreeNode *z = new RBTreeNode(y, sentinel, sentinel, value);

	if (y == sentinel) {
		root = z;
	} else if (value < y->value) {
		y->left = z;
	} else {
		y->right = z;
	}

	insert_fixup(z);

	return z;
}

RBTreeNode* subtree_minimum(RBTreeNode *x) {
	while (!x->left->sentinel) {
		x = x->left;
	}
	return x;
}

RBTreeNode* subtree_maximum(RBTreeNode *x) {
	while (!x->right->sentinel) {
		x = x->right;
	}
	return x;
}

RBTreeNode* RBTree::find_min() {
	return subtree_minimum(root);
}

void RBTree::remove_min() {
	remove(subtree_minimum(root));
}

bool RBTree::remove(int key) {
	return remove(get(key));
}

bool RBTree::remove(RBTreeNode *z) {
	RBTreeNode *x;

	RBTreeNode *y = z;
	bool y_original_red = y->red;
	
	if (z->left == sentinel) {
		x = z->right;
		transplant(z, z->right);
	} else if (z->right == sentinel) {
		x = z->left;
		transplant(z, z->left);
	} else {
		y = subtree_minimum(z->right);
		y_original_red = y->red;
		x = y->right;
		if (y->p == z) {
			x->p = y;
		} else {
			transplant(y, y->right);
			y->right = z->right;
			y->right->p = y;
		}
		transplant(z, y);
		y->left = z->left;
		y->left->p = y;
		y->red = z->red;
	}
	if (!y_original_red) {
		remove_fixup(x);
	}

	if (root == sentinel) {
		root = nullptr;
	}

	return true;
}

void RBTree::remove_fixup(RBTreeNode *x) {
	while(x != root && !x->red) {
		if (x == x->p->left) {
			RBTreeNode *w = x->p->right;
			if (w->red) {
				w->red = false;
				x->p->red = true;
				rotate_left(x->p);
				w = x->p->right;
			}
			if (!w->left->red && !w->right->red) {
				w->red = true;
				x = x->p;
			} else {
				if (!w->right->red) {
					w->left->red = false;
					w->red = true;
					rotate_right(w);
					w = x->p->right;
				}
				w->red = x->p->red;
				x->p->red = false;
				w->right->red = false;
				rotate_left(x->p);
				x = root;
			}
		} else {
			RBTreeNode *w = x->p->left;
			if (w->red) {
				w->red = false;
				x->p->red = true;
				rotate_right(x->p);
				w = x->p->left;
			}
			if (!w->left->red && !w->right->red) {
				w->red = true;
				x = x->p;
			} else {
				if (!w->left->red) {
					w->right->red = false;
					w->red = true;
					rotate_left(w);
					w = x->p->left;
				}
				w->red = x->p->red;
				x->p->red = false;
				w->left->red = false;
				rotate_right(x->p);
				x = root;
			}
		}
	}

	x->red = false;
}

RBTreeNode* RBTree::predecessor(RBTreeNode *z) {
	// If z has a left child, its predecessor is the max node in that subtree
	if (z->left != sentinel) {
		return subtree_maximum(z->left);
	}

	// If z does not have a left child, its predecessor is its first left ancestor
	RBTreeNode *res = z->p;
	while (res != sentinel && z != res->right) {
		z = res;
		res = res->p;
	}
	return (res == sentinel) ? nullptr : res;
}

void print_aux(RBTreeNode *n, int indent) {
	// Print sentinel, much special, wow, so amaze
	if (n->sentinel) {
		//printf("%*sNIL\n", indent, " ");
		return;
	}

	int p = n->p->value;
	int l = n->left->value;
	int r = n->right->value;
	const char *color = (n->red) ? "RED" : "BLACK";

	cout << setw(indent);
	printf("%*s%d [p=%d, l=%d, r=%d] %s\n", indent, " ", n->value, p, l, r, color);

	print_aux(n->left, indent + 4);
	print_aux(n->right, indent + 4);
}

void RBTree::print() {
	cout << endl << "=============================\nPrinting tree\n=============================" << endl;
	print_aux(root, 0);
	cout << endl << endl;
}

void RBTree::insert_fixup(RBTreeNode *z) {
	RBTreeNode *y;
	while (z->p->red) {
		if (z->p == z->p->p->left) {
			y = z->p->p->right;
			if (y->red) {
				z->p->red = false;
				y->red = false;
				z->p->p->red = true;
				z = z->p->p;
			} else {
				if (z == z->p->right) {
					z = z->p;
					rotate_left(z);
				}
				z->p->red = false;
				z->p->p->red = true;
				rotate_right(z->p->p);
			}
		} else {
			// Same as then case, with left and right swapped
			y = z->p->p->left;
			if(y->red) {
				z->p->red = false;
				y->red = false;
				z->p->p->red = true;
				z = z->p->p;
			} else {
				if (z == z->p->left) {
					z = z->p;
					rotate_right(z);
				}
				z->p->red = false;
				z->p->p->red = true;
				rotate_left(z->p->p);
			}
		}
	}

	root->red = false;
}

void RBTree::rotate_left(RBTreeNode *x) {
	RBTreeNode *y = x->right;
	
	// Turn y's left subtree into x's right subtree
	x->right = y->left;
	if (y->left != sentinel) y->left->p = x;

	// Link x's parent to y
	y->p = x->p;
	if (x->p == sentinel)      root = y;
	else if (x == x->p->left) x->p->left  = y;
	else                           x->p->right = y;

	// Put x on y's left
	y->left   = x;
	x->p = y;
};

void RBTree::rotate_right(RBTreeNode *y) {
	RBTreeNode *x = y->left;

	// Turn x's right subtree into y's left subtree
	y->left = x->right;
	if (x->right != sentinel) x->right->p = y;

	// Link y's parent to x
	x->p = y->p;
	if (y->p == sentinel)      root = x;
	else if (y == y->p->left) y->p->left  = x;
	else                           y->p->right = x;

	// Put y on x's right
	x->right  = y;
	y->p = x;
};

void RBTree::transplant(RBTreeNode *u, RBTreeNode *v) {
	if (u->p == sentinel) {
		root = v;
	} else if (u == u->p->left) {
		u->p->left = v;
	} else {
		u->p->right = v;
	}

	v->p = u->p;
}

bool check4aux(RBTreeNode *n) {
	if (n->sentinel) return true;
	if (n->red && (n->left->red || n->right->red)) return false;
	return check4aux(n->left) && check4aux(n->right);
}

// Counts number of black nodes to a leaf
// Returns the number of black nodes to a leaf, or -1 when different paths
// has a different number of black nodes
int check5aux(RBTreeNode *n) {
	if (n->sentinel) return 1;

	int leftcount  = check5aux(n->left);
	int rightcount = check5aux(n->right);

	if (leftcount == rightcount && leftcount != -1) {
		if (n->red) return leftcount;
		else 		return leftcount + 1;
	} else {
		// There is an error !!
		return -1;
	}
}

bool RBTree::validate() {
	cout << "VALIDATING TREE" << endl;

	// 1. Every node is either red or black
	// Trivial

	// 2. The root is black
	if (root->red) {
		cout << "Property 2 violated" << endl;
		return false;
	}

	// 3. Every leaf (NIL) is black
	if (sentinel->red) {
		cout << "Property 3 violated" << endl;
		return false;
	}
	
	// 4. If a node is red then both its children are black
	if (!check4aux(root)) {
		cout << "Property 4 violated" << endl;
		return false;
	}

	// 5. For each node, all simple paths from the node 
	//    to descendant leaves contain the same number of black nodes.
	if (check5aux(root) == -1) {
		cout << "Property 5 violated" << endl;
		return false;
	}

	cout << "Validation successful" << endl;
	return true;
}