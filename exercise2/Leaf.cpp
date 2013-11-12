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
    /*
     * e[0] e[1] | min max
     * ----------+--------
     *   T    F  |  0   0
     *   T    T  |  0   1
     *   F    F  |  n  -1
     *   F    T  |  1   1
     */
	_size -= 1;
    if (entries[0] && !entries[1]) {
        _min = 0;
        _max = 0;
    } else if (entries[0] && entries[1]) {
        _min = 0;
        _max = 1;
    } else if (!entries[0] && !entries[1]) {
        _min = _n;
        _max = -1;
    } else if (!entries[0] && entries[1]) {
        _min = 1;
        _max = 1;
    }
}

int Leaf::succ(int i) {
    if (i == 0 && entries[0]) return 0;
    else if (entries[1]) return 1;
    return -1;
}

