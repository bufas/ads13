#include <stdio.h>
#include <stdlib.h>

typedef struct node node;
struct node {
	node* parent;           // Parent node
    node* child;            // Any child node
    node* left_sibling;     // Left sibling
    node* right_sibling;    // Right sibling
    int key;                // Priority
    int rank;               // Rank >= 0
    int marked;             // Unmarked = 0, marked = 1
};
typedef node* heap;

heap meld(heap h1, heap h2) {
	if (h1 == NULL) return h2;
	if (h2 == NULL) return h1;

	// For simplicity, we assume that h1 has the lowest priority key, and if
	// this is not the case, we swap the pointers, so it becomes the case.
	if (h2->key < h1->key) {
		// swap h1 and h2
		heap tmp = h1;
		h1 = h2;
		h2 = tmp;
	}

	h2->parent = h1;
	if (h1->child == NULL) {
		// h1 has no child, so h2 becomes only child
		h1->child = h2;
	} else {
		// h1 has children, so we need to update pointers
		node child = *(h1->child);
		if (child.left_sibling == NULL) {
			// h1 has only a single child
			child.left_sibling = h2;
			child.right_sibling = h2;
			h2->left_sibling = &child;
			h2->right_sibling = &child;
		} else {
			// h1 has multiple children, insert between child and its left sibling
			node child2 = *(child.left_sibling);

			h2->left_sibling = &child2;
			h2->right_sibling = &child;

			child.left_sibling = h2;
			child2.right_sibling = h2;
		}
	}
	return h1;
}

// heap make_heap() {
// 	heap h = malloc(sizeof(node));
// 	h->key = 0;
// 	h->rank = 0;
// 	h->marked = 0;
// 	h->parent = NULL;
// 	h->child = NULL;
// 	h->left_sibling = NULL;
// 	h->right_sibling = NULL;
// 	return h;
// }
heap make_heap() {
	return NULL;
}

node find_min(heap h) {
	return *h;
}

void insert(int key, heap *h) {
    node n;
    n.key = key;

	*h = meld(*h, &n);
}

void print_heap(heap h) {
	static int offset = 0;

	for (int i = 0; i < offset; i++) {
		printf(" ");
	}

	if (h == NULL) {
		printf("-\n");
		return;
	}

	printf("%d\n", h->key);

	offset += 2;

	node* first = h->child;
	if (first == NULL) return;

	node* next = h->child;
	do {
		print_heap(next);
		printf("bob\n");
		next = next->right_sibling;
		printf("frank\n");
	} while (next != first);

	offset -= 2;
}

void add(int **ptr1, int *ptr2) {
	int i = 2;
	*ptr1 = &i;
}

int main(void) {
	heap h = make_heap();
	insert(8, &h);
	// insert(6, &h);
	// insert(5, h);
	// insert(7, h);
	print_heap(h);

    return 0; 
}
