#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "RBTree.h"

using namespace std;

template<typename T>
RBTreeNode<T>* RBTree<T>::get(int key) {
	RBTreeNode<T> *x = root;
	while (x != nullptr && x->key != key) {
		x = (key > x->key) ? x->right : x->left;
	}
	return x;
}

template<typename T>
RBTreeNode<T>* RBTree<T>::insert(int key, T value) {
	RBTreeNode<T> *y = nullptr;
	RBTreeNode<T> *x = root;

	while (x != nullptr) {
		y = x;
		if (key < x->key) {
			x = x->left;
		} else {
			x = x->right;
		}
	}

	RBTreeNode<T> *z = new RBTreeNode<T>(y, key, value);

	if (y == nullptr) {
		root = z;
	} else if (key < y->key) {
		y->left = z;
	} else {
		y->right = z;
	}

	insert_fixup(z);
	size++;
	return z;
}

template<typename T>
RBTreeNode<T>* subtree_minimum(RBTreeNode<T> *x) {
	while (x->left != nullptr) {
		x = x->left;
	}
	return x;
}

template<typename T>
RBTreeNode<T>* subtree_maximum(RBTreeNode<T> *x) {
	while (x->right != nullptr) {
		x = x->right;
	}
	return x;
}

template<typename T>
RBTreeNode<T>* RBTree<T>::find_min() {
	return subtree_minimum(root);
}

template<typename T>
void RBTree<T>::remove_min() {
	remove(subtree_minimum(root));
}

template<typename T>
bool RBTree<T>::remove(int key) {
	return remove(get(key));
}

template<typename T>
bool RBTree<T>::remove(RBTreeNode<T> *z) {
	if (z == nullptr) {
		cout << "null..." << endl;
		return false;
	}
	RBTreeNode<T> *x;

	RBTreeNode<T> *y = z;
	bool y_original_red = y->red;
	
	if (z->left == nullptr) {
		x = z->right;
		transplant(z, z->right);
	} else if (z->right == nullptr) {
		x = z->left;
		transplant(z, z->left);
	} else {
		y = subtree_minimum(z->right);
		y_original_red = y->red;
		x = y->right;
		if (y->p == z) {
			// x->p = y;
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

	size--;
	return true;
}

template<typename T>
void RBTree<T>::remove_fixup(RBTreeNode<T> *x) {
	while(x != root && x != nullptr && !x->red) {
		if (x == x->p->left) {
			RBTreeNode<T> *w = x->p->right;
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
			RBTreeNode<T> *w = x->p->left;
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

	if (x != nullptr) {
		x->red = false;
	}
}

template<typename T>
int RBTree<T>::predecessor(int x) {
	RBTreeNode<T> *z = get(x);

	// If z has a left child, its predecessor is the max node in that subtree
	if (z->left != nullptr) {
		return subtree_maximum(z->left)->key;
	}

	// If z does not have a left child, its predecessor is its first left ancestor
	RBTreeNode<T> *res = z->p;
	while (res != nullptr && z != res->right) {
		z = res;
		res = res->p;
	}
	return (res == nullptr) ? -1 : res->key;
}

template<typename T>
void print_aux(RBTreeNode<T> *n, int indent) {
	// Print sentinel, much special, wow, so amaze
	if (n == nullptr) {
		//printf("%*sNIL\n", indent, " ");
		return;
	}

	int p = (n->p     != nullptr) ? n->p->key     : -1;
	int l = (n->left  != nullptr) ? n->left->key  : -1;
	int r = (n->right != nullptr) ? n->right->key : -1;
	const char *color = (n->red) ? "RED" : "BLACK";

	cout << setw(indent);
	printf("%*s%d [p=%d, l=%d, r=%d] %s\n", indent, " ", n->key, p, l, r, color);

	print_aux(n->left, indent + 4);
	print_aux(n->right, indent + 4);
}

template<typename T>
void RBTree<T>::print() {
	cout << endl << "=============================\nPrinting tree\n=============================" << endl;
	print_aux(root, 0);
	cout << endl << endl;
}

template<typename T>
void print_inorder_aux(RBTreeNode<T> *n) {
	cout << n->key;
}

template<typename T>
void RBTree<T>::print_inorder() {
	inorder(print_inorder_aux);
	cout << endl;
}

template<typename T>
void inorder_aux(RBTreeNode<T> *n, void (*f)(RBTreeNode<T> *n)) {
	if (n == nullptr) return;

	inorder_aux(n->left, f);
	f(n);
	inorder_aux(n->right, f);
}

template<typename T>
void RBTree<T>::inorder(void (*f)(RBTreeNode<T> *n)) {
	cout << endl << "=============================[ in-order tree walk ]=============================" << endl;
	inorder_aux(root, f);
}

template<typename T>
void RBTree<T>::insert_fixup(RBTreeNode<T> *z) {
	RBTreeNode<T> *y;
	while (z->p != nullptr && z->p->red) {
		if (z->p == z->p->p->left) {
			y = z->p->p->right;
			if (y != nullptr && y->red) {
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
			if (y != nullptr && y->red) {
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

template<typename T>
void RBTree<T>::rotate_left(RBTreeNode<T> *x) {
	RBTreeNode<T> *y = x->right;
	
	// Turn y's left subtree into x's right subtree
	x->right = y->left;
	if (y->left != nullptr) y->left->p = x;

	// Link x's parent to y
	y->p = x->p;
	if (x->p == nullptr)      root = y;
	else if (x == x->p->left) x->p->left  = y;
	else                      x->p->right = y;

	// Put x on y's left
	y->left   = x;
	x->p = y;
};

template<typename T>
void RBTree<T>::rotate_right(RBTreeNode<T> *y) {
	RBTreeNode<T> *x = y->left;

	// Turn x's right subtree into y's left subtree
	y->left = x->right;
	if (x->right != nullptr) x->right->p = y;

	// Link y's parent to x
	x->p = y->p;
	if (y->p == nullptr)      root = x;
	else if (y == y->p->left) y->p->left  = x;
	else                      y->p->right = x;

	// Put y on x's right
	x->right  = y;
	y->p = x;
};

template<typename T>
void RBTree<T>::transplant(RBTreeNode<T> *u, RBTreeNode<T> *v) {
	if (u->p == nullptr) {
		root = v;
	} else if (u == u->p->left) {
		u->p->left = v;
	} else {
		u->p->right = v;
	}

	if (v != nullptr) {
		v->p = u->p;
	}
}

template<typename T>
bool check4aux(RBTreeNode<T> *n) {
	if (n == nullptr) return true;
	if (n->red && ((n->left != nullptr && n->left->red) || (n->right != nullptr && n->right->red))) return false;
	return check4aux(n->left) && check4aux(n->right);
}

// Counts number of black nodes to a leaf
// Returns the number of black nodes to a leaf, or -1 when different paths
// has a different number of black nodes
template<typename T>
int check5aux(RBTreeNode<T> *n) {
	if (n == nullptr) return 1;

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

template<typename T>
bool RBTree<T>::validate() {
	cout << "VALIDATING TREE" << endl;

	// 1. Every node is either red or black
	// Trivial

	// 2. The root is black
	if (root->red) {
		cout << "Property 2 violated" << endl;
		return false;
	}

	// 3. Every leaf (NIL) is black
	// NOTE: leaves are now nullptr
	// if (sentinel != nullptr) {
	// 	cout << "Property 3 violated" << endl;
	// 	return false;
	// }
	
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