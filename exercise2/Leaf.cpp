#include "Leaf.hpp"
#include <assert.h>

Leaf::Leaf() : ITree(2) {
	entries[0] = false;
	entries[1] = false;
}

void Leaf::insert(int i) {
	assert(entries[i] != true);
	entries[i] = true;

	// Update fields
    if (i < _min) _min = i;
    if (i > _max) _max = i;
	_size += 1;
}

void Leaf::remove(int i) {
	assert(entries[i] == true);
	entries[i] = false;

	// Update fields
    if (i < _min) _min = i;
    if (i > _max) _max = i;
	_size -= 1;
}

int Leaf::succ(int i) {
    return -1; // TODO
}

