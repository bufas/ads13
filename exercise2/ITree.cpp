#include <cmath>
#include "ITree.hpp"

ITree::ITree(int n) : _size(0), _max(-1), _min(n), _n(n) {
	float real_root = std::sqrt(_n);

    // Take sqrt and reound to nearest power of 2
	int pre = 1;
	int cur = 1;
	while (cur < real_root) {
		pre = cur;
		cur *= 2;
	}

	_sqrtn = cur; // Always overestimate
	//_sqrtn = (cur - real_root > real_root - pre) ? pre : cur;
}

int ITree::getSize() {
    return _size;
}
