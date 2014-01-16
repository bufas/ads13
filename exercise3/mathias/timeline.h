
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <set>

#include "retro.h"
#include "rb-tree/RBTree.h"


using namespace std;

class Node;
class Leaf;
class Internal;
class Tree;



class Node {
	
	public:

		Internal *parent;
		set<int> elements;
		int size;
		bool leaf;

		Node(Internal *parent, int size, bool leaf) 
			: parent(parent), size(size), leaf(leaf) {}

		virtual Internal* insert(int t, Op o, int key, Node *leaf) = 0;
		virtual void print(int indent) = 0;
		virtual bool member_search(int key) = 0;

};

class Leaf : public Node {

	public:

		Op o;
		int x;
		// If o is INSERT this is nullptr, but if o is DELETE, this
		// field links to the node where the object x was inserted.
		Leaf *insert_leaf;

		Leaf(Internal *p, Op o, int x) : Node(p, 1, true), o(o), x(x), insert_leaf(nullptr) {}

		Internal* insert(int t, Op o, int key, Node *leaf);
		void print(int indent);
		void invalidate();
		bool member_search(int key);

};

//***********************************************************************************************//
//**                                                                                           **//
//**                                                                                           **//
//**                                     THIS IS INTERNAL                                      **//
//**                                                                                           **//
//**                                                                                           **//
//***********************************************************************************************//


class Internal : public Node {

	public:

		Node *child[3];

		Internal(Internal *p, Node *n1, Node *n2) 
				: Node(p, n1->size + n2->size, false) {
			child[0] = n1;
			child[1] = n2;
			child[2] = nullptr;

			n1->parent = this;
			n2->parent = this;
		}


		Node* get_rightmost() {
			return (child[2] == nullptr) ? child[1] : child[2];
		}

		Node* get_leftmost() {
			return child[0];
		}

		// Returns a pair containing the index of the child to decend into
		// and what the time should be for the recursive call.
		pair<int, int> descent_helper(int t) {
			for (int i = 0; i < 3; i++) {
				if (t < child[i]->size) return make_pair(i, t);
				t -= child[i]->size;
			}

			// This should never ever happen			
			std::cout << "ARE YOU A HORSE????" << std::endl;
			return make_pair(-1,-1);
		}

		Internal* insert(int t, Op o, int key, Node *leaf) {
			// Remove the child
			if (t < child[0]->size) {
				return child[0]->insert(t, o, key, leaf);
			} 
			else if (t < child[1]->size + child[0]->size) {
				t -= child[0]->size;
				return child[1]->insert(t, o, key, leaf);
			} 
			else if (child[2] != nullptr) {
				t -= child[0]->size + child[1]->size;
				return child[2]->insert(t, o, key, leaf);
			}
			else {
				// Inserting at current time
				leaf->parent = this;
				child[2] = leaf;
				size++;
				return nullptr;
			}
		}

		Internal* insert_child_before(Node* insert_before, Node* new_child) {
			Node *appendix = nullptr;
			if (child[0] == insert_before) {
				appendix = child[2];
				child[2] = child[1];
				child[1] = child[0];
				child[0] = new_child;
			}
			else if (child[1] == insert_before) {
				appendix = child[2];
				child[2] = child[1];
				child[1] = new_child;
			}
			else if (child[2] == insert_before) {
				appendix = child[2];
				child[2] = new_child;
			}
			else {
				printf("Dart Waldorf\n");
			}

			if (appendix != nullptr) {
				Internal *split_node = split(appendix);
				return parent->insert_child_after(this, split_node);
			}
			return nullptr;
		}

		Internal* insert_child_after(Node* insert_after, Node* new_child) {
			Node *appendix = nullptr;
			if (child[0] == insert_after) {
				appendix = child[2];
				child[2] = child[1];
				child[1] = new_child;
			}
			else if (child[1] == insert_after) {
				appendix = child[2];
				child[2] = new_child;
			}
			else if (child[2] == insert_after) {
				appendix = new_child;
			}
			else {
				printf("Dart Waldorf 2\n");
			}

			if (appendix != nullptr) {
				Internal *split_node = split(appendix);
				if (parent == nullptr) {
					// Create new root
					return new Internal(nullptr, this, split_node);
				} else {
					return parent->insert_child_after(this, split_node);
				}
			}
			return nullptr;
		}

		set<int> get_intersection(set<int> e1, set<int> e2) {
			set<int> result;
			std::set_intersection(e1.begin(), e1.end(), e2.begin(), e2.end(), insert_iterator<set<int>>(result, result.begin()));
			return result;
		}

		void elements_insert(Node *n, set<int> set) {
			n->elements.insert(set.begin(), set.end());
		}

		void elements_erase(Node *n, set<int> set) {
			// cout << "elements_erase " << "{ ";
			// for (auto it=set.begin(); it!=set.end(); ++it) printf("%d ", *it);
			// cout << "}" << " from { ";
			// for (auto it=n->elements.begin(); it!=n->elements.end(); ++it) printf("%d ", *it);
			// cout << "}" << endl;;

			for (auto it=set.begin(); it!=set.end(); ++it) {
				n->elements.erase(*it);
			}
		}

		// Precondition: n has 3 children
		Internal* split(Node *n) {
			Node *tmp = child[2];
			Internal *new_node = new Internal(parent, tmp, n);

			// Move elements around (order of steps is important)
			//   1. copy elements from this to new_node
			//   2. intersection of child[0], child[1], and child[2] should be removed from child[1] and child[2] and added to this
			//   3. intersection of child[1], child[2], and n should be removed from child[2] and n and added to new_node
			//   4. intersection of child[0] and child[1] should be pushed to this
			//   5. intersection of child[2] and n should be pushed to new_node

			// Step 1.
			elements_insert(new_node, elements);

			// Step 2.
			set<int> part_intersect = get_intersection(child[0]->elements, child[1]->elements);
			set<int> full_intersect = get_intersection(part_intersect, child[2]->elements);
			elements_insert(this, full_intersect);
			elements_erase(child[1], full_intersect);
			elements_erase(child[2], full_intersect);

			// Step 3.
			part_intersect = get_intersection(child[1]->elements, child[2]->elements);
			full_intersect = get_intersection(part_intersect, n->elements);
			elements_insert(new_node, full_intersect);
			elements_erase(child[2], full_intersect);
			elements_erase(n, full_intersect);

			// Step 4.
			full_intersect = get_intersection(child[0]->elements, child[1]->elements);
			elements_insert(this, full_intersect);
			elements_erase(child[0], full_intersect);
			elements_erase(child[1], full_intersect);

			// Step 5.
			full_intersect = get_intersection(child[2]->elements, n->elements);
			elements_insert(new_node, full_intersect);
			elements_erase(child[2], full_intersect);
			elements_erase(n, full_intersect);

			// Detatch child[2] from this, as it now belongs to new_node
			child[2] = nullptr;
			update_size(); // TODO this should be unnessessary

			return new_node;
		}

		void elements_pull(Node *source, Node *target) {
			// We should pull only those elements that is not deleted in the
			// subtree in which they reside
			int skip_value = -1;
			// Check if rightmost operation is delete. In this case we should
			// not pull the key that is deleted
			Node *n = source;
			while(!n->leaf) n = ((Internal*) n)->get_leftmost();
			Leaf *rightmost_leaf = (Leaf*) n;
			if (rightmost_leaf->o == DELETE) skip_value = rightmost_leaf->x;

			// Pull all elements except the skip_value
			for (auto it=source->elements.begin(); it!=source->elements.end(); ++it) {
				if (*it != skip_value) target->elements.insert(*it);
			}
		}

		Internal* append_child(Node *n) {
			n->parent = this;
			
			// Check if there is room for one more node
			if (child[2] == nullptr) {
				child[2] = n;
				size += n->size;

				elements_insert(n, this->child[1]->elements);

				Internal *n = this;
				while (n != nullptr) {
					n->update_size();
					n = n->parent;
				}

				return nullptr;
			}

			//
			// Split
			//

			// Pull all the elements from the left siblings to reach a consistent state before splitting
			elements_pull(this->child[1], n);
			elements_pull(this->child[2], n);
			Internal *sibling = split(n);

			if (parent == nullptr) {
				Internal *new_root = new Internal(nullptr, this, sibling);

				// The intersection of the elements in this and sibling must be
				// pushed up into new_root (I know I don't have to use explicit this)
				set<int> intersection = get_intersection(this->elements, sibling->elements);
				elements_insert(new_root, intersection);
				elements_erase(this, intersection);
				elements_erase(sibling, intersection);

				return new_root;
			} else {
				return parent->append_child(sibling);
			}
		}

		void update_size() {
			size = 0;
			for (int i = 0; i < 3; i++) {
				if (child[i] != nullptr) size += child[i]->size;
			}
		}

		void print(int indent) {
			printf("Internal [s=%d] elems{ ", size);
			// Also print the elements
			for (auto it=elements.begin(); it!=elements.end(); ++it) {
				printf("%d ", *it);
			}
			cout << "}" << endl;

			for (int i = 0; i < 3; i++) {
				printf("%*schild[%d]: ", indent + 2, " ", i);
				if (child[i] != nullptr) child[i]->print(indent + 4);
				else printf("NULL\n");
			}
		}

		// TODO this function is duplicated
		// This function finds the predecessor, by performing a
		// predecessor search in all secondary structures along
		// the path to the root.
		bool member_search(int key) {
			bool mem = elements.find(key) != elements.end();
			if (parent == nullptr) return mem;
			return parent->member_search(key) || mem;
		}

};

//***********************************************************************************************//
//**                                                                                           **//
//**                                                                                           **//
//**                                       THIS IS LEAF                                        **//
//**                                                                                           **//
//**                                                                                           **//
//***********************************************************************************************//


	Internal* Leaf::insert(int t, Op o, int key, Node *leaf) {
		leaf->parent = parent;
		return parent->insert_child_before(this, leaf);
	}

	void Leaf::print(int indent) {
		printf("%s(%d) elems{ ", (o == INSERT ? "insert" : "delete"), x);
		// Also print the elements
		for (auto it=elements.begin(); it!=elements.end(); ++it) {
			printf("%d ", *it);
		}
		cout << "}" << endl;
	}

	// This function is supposed to null its contents and
	// run to the root, decrementing the node sizes by 1,
	// as it is no longer officially part of the tree.
	void Leaf::invalidate() {
		Node *n = this;
		while (n != nullptr) {
			n->size--;
			n = n->parent;
		}
	}

	// TODO this function is duplicated
	// This function finds the predecessor, by performing a
	// predecessor search in all secondary structures along
	// the path to the root.
	bool Leaf::member_search(int key) {
		bool mem = elements.find(key) != elements.end();
		if (parent == nullptr) return mem;
		return parent->member_search(key) || mem;
	}


//***********************************************************************************************//
//**                                                                                           **//
//**                                                                                           **//
//**                                       THIS IS TREE                                        **//
//**                                                                                           **//
//**                                                                                           **//
//***********************************************************************************************//


class Tree {

	public:

		Node *root;
		int size;

		Tree() : root(nullptr), size(0) {}

		// // Precondition, x != size
		// int predecessor(int t, int x) {
		// 	int result = root->elements->predecessor(x);

		// 	// Descend to the leaf representing the time
		// 	Node *n = root;
		// 	while (!n->leaf) {
		// 		Internal *ni = (Internal *) n;

		// 		pair<int, int> dh = ni->descent_helper(t);
		// 		n = ni->child[dh.first];
		// 		t = dh.second;

		// 		// Find predecessor for this node and set result if this is larger
		// 		int this_pred = n->elements->predecessor(x);
		// 		if (this_pred > result) result = this_pred;
		// 	}

		// 	return result;
		// }

		// Leaf* insert(Op o, int key) {
		// 	return insert(size, o, key);
		// }

		Leaf* insert(int t, Op o, int key) {
			Leaf *leaf = new Leaf(nullptr, o, key);

			// Special cases for sizes 0 and 1
			if (size == 0) {
				root = leaf;
			}
			else if (size == 1) {
				if (t == 0) {
					root = new Internal(nullptr, leaf, root);
				} else {
					Leaf *tmp = (Leaf *) root;
					Node *n = new Internal(nullptr, tmp, leaf);
					n->elements.insert(tmp->x);
					tmp->elements.clear();
					root = n;
				}
			} 
			else {
				if (t == size) {
					// Insert at current time
					Internal *prev = nullptr;
					Node *n = root;
					while (!n->leaf) {
						prev = (Internal *) n;
						n = prev->get_rightmost();
					}
					Internal *res = prev->append_child(leaf);
					if (res != nullptr) root = res;
				} else {
					Internal *res = ((Internal *) root)->insert(t, o, key, leaf);
					if (res != nullptr) {
						root = res;
					}
				}

			}

			size++;
			return leaf;
		}

		void restore_interval_from(Leaf *from, int key) {

		}

		void insert_interval_to_end(Leaf *from, int key) {
			// Step 1, push the key as high up as possible
			Node *n = from;
			while(n->parent != nullptr && n == n->parent->get_leftmost()) {
				n = n->parent;
			}
			n->elements.insert(key);

			// Step 2, insert in all successor trees.
			// This is done by inserting in all right siblings, and proceeding one level up.
			while(n->parent != nullptr) {
				// Insert in all right siblings
				if (n == n->parent->child[0]) {
					// We are the leftmost child, insert in child[1] and child[2] (if exists)
					n->parent->child[1]->elements.insert(key);
					if (n->parent->child[2] != nullptr) {
						n->parent->child[2]->elements.insert(key);
					}
				}
				else if (n == n->parent->child[1]) {
					// We are child[1]. Insert into child[2] if it exists
					if (n->parent->child[2] != nullptr) {
						n->parent->child[2]->elements.insert(key);
					} 
				}

				n = n->parent;
			}
		}

		bool elements_contains(set<int> e, int key) {
			return e.find(key) != e.end();
		}

		void stop_interval(Leaf *to, int key) {
			bool pushed_to_key = false;

			// Step 1, push the key as high up as possible
			Node *n = to;
			while(n->parent != nullptr && n == n->parent->get_rightmost()) {
				// The element is already present at this node
				if (elements_contains(n->elements, key)) {
					pushed_to_key = true;
					break;
				}
				n = n->parent;
			}
			n->elements.insert(key); // it might already be there

			// If we have pushed the key all the way to where it is,
			// we should go to step 3.
			if (!pushed_to_key) {
				// Step 2, insert x in all left siblings unless key is in n
				while (!elements_contains(n->elements, key) && n->parent != nullptr) {
					if (n == n->parent->child[2]) {
						n->parent->child[1]->elements.insert(key);
						n->parent->child[0]->elements.insert(key);
					} else if (n == n->parent->child[1]) {
						n = n->parent->child[0];
					}
					n = n->parent;
				}

				// Remove key from n, and remove it from right siblings
				n->elements.erase(key);
				if (n->parent != nullptr) {
					if (n == n->parent->child[0]) {
						n->parent->child[1]->elements.erase(key);
						if (n->parent->child[2] != nullptr) n->parent->child[2]->elements.erase(key);
					} else if (n == n->parent->child[1]) {
						if (n->parent->child[2] != nullptr) n->parent->child[2]->elements.erase(key);
					}
				}
			}

			// Go to parent, and from this point, just remove key
			// from all right siblings
			n = n->parent;

			// Step 3, n might be null at this point...
			if (n == nullptr) return;

			// Step 4, remove key from all successor trees
			while(n->parent != nullptr) {
				// Remove from all right siblings
				if (n == n->parent->child[0]) {
					// We are the leftmost child, remove from child[1] and child[2] (if exists)
					n->parent->child[1]->elements.erase(key);
					if (n->parent->child[2] != nullptr) {
						n->parent->child[2]->elements.erase(key);
					}
				}
				else if (n == n->parent->child[1]) {
					// We are child[1]. Remove from child[2] if it exists
					if (n->parent->child[2] != nullptr) {
						n->parent->child[2]->elements.erase(key);
					}
				}

				n = n->parent;
			}

		}

		void print() {
			if (root == nullptr) return;
			root->print(0);
			std::cout << std::endl;
		}
};








		// void insert_interval(Leaf *from, Leaf *to, int key) {
		// 	// First, push the key as high up in the tree as possible
		// 	Node *n = from;
		// 	while(n == n->parent->get_leftmost()) n = n->parent;
		// 	n->elements.insert(key);

		// 	// Second, insert in all successor trees.
		// 	// This is done by inserting in all right siblings, and proceeding one
		// 	// level up. We stop when we reach the root or when a right sibling
		// 	// contains the DELETE operation. 
		// 	while(n->parent != nullptr) {
		// 		// Insert in all right siblings
		// 		if (n == n->parent->child[0]) {
		// 			// We are the leftmost child, insert in child[1] and child[2], 
		// 			// unless "to" resides in one of those subtrees.
		// 			if (!n->parent->child[1]->contains(to)) {
		// 				n->parent->child[1]->elements.insert(key);
		// 				if (n->parent->child[2] != nullptr) {
		// 					if (!n->parent->child[2]->contains(to)) {
		// 						n->parent->child[2]->elements.insert(key);
		// 					} else {
		// 						break;
		// 					}
		// 				}
		// 			} else {
		// 				// If it does, we are done
		// 				break;
		// 			}
		// 		}
		// 		else if (n == n->parent->child[1]) {
		// 			// We are child[1]. Insert into child[2] if it exists and
		// 			// does not contain "to".
		// 			if (n->parent->child[2] != nullptr) {
		// 				if (!n->parent->child[2]->contains(to)) {
		// 					n->parent->child[2]->elements.insert(key);
		// 				} else {
		// 					break;
		// 				}
		// 			} 
		// 		}

		// 		n = n->parent;
		// 	}

		// 	// We need to do the same from "to", by inserting in all left siblings
		// 	Node *n = from;
		// 	while(n == n->parent->get_rightmost()) n = n->parent;
		// 	n->elements.insert(key);

		// }

		// void remove(int t) {
		// 	// Check for invalid argument
		// 	if (t < 0 || t >= size) {
		// 		std::cout << "You, Sir, suck." << std::endl;
		// 		return;
		// 	}

		// 	// Special cases for sizes 0 and 1
		// 	if (size == 1) {
		// 		root = nullptr;
		// 	}
		// 	else if (size == 2) {
		// 		Internal *r = (Internal *) root;
		// 		root = r->child[1-t];
		// 		root->parent = nullptr;
		// 	} 
		// 	else {
		// 		((Internal *) root)->remove(t);
		// 	}

		// 	size--;
		// }

