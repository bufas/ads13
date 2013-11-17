#include "Tree.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits.h>

Tree::Tree(int n) : _size(0), _max(-1), _min(n), _n(n), _top(nullptr) {
    float real_root = std::sqrt(_n);

    // Take sqrt and round up to nearest power of 2
    _sqrtn = 1;
    while (_sqrtn < real_root) _sqrtn *= 2;

    _bottom = new Tree*[_sqrtn];
    std::fill(_bottom, _bottom + _sqrtn, nullptr);
}

Tree::~Tree() {
    delete _top;
    for (int i = 0; i < _sqrtn; i++) {
        delete _bottom[i];
    }
    delete[] _bottom;
}

// TODO size gets update even if the same index is inserted twice
void Tree::insert(int i) {
    if (_size == 0) {
        _size++;
        _max = _min = i;
        return;
    }

    if (_min > i) {
        int tmp = i;
        i       = _min;
        _min    = tmp;
    }

    int a = i / _sqrtn;
    int b = i % _sqrtn;

    // Insert in top and bottom trees
    if (_bottom[a] == nullptr) _bottom[a] = new Tree(_sqrtn);
    if (_bottom[a]->_size == 0) {
        if (_top == nullptr) _top = new Tree(_sqrtn);
        _top->insert(a);
    }
    _bottom[a]->insert(b);

    // Update fields
    _size++;
    if (i < _min) _min = i;
    if (i > _max) _max = i;
}

// TODO handle when trying to delete elements that are not in the tree
void Tree::remove(int i) {
    if (_size == 0) return;
    if (_size == 1 && _min != i) return;

    if (_size == 1) {
        _size--;
        _min = _n;
        _max = -1;
        return;
    }

    // The smallest element are not physically in the tree. It is just represented
    // by _min. When this is deleted, we need to find the new min element, set _min
    // and delete the new _min from the tree.
    if (_min == i) {
        _min = _top->_min * _sqrtn + _bottom[_top->_min]->_min;
        i = _min;
    }

    int a = i / _sqrtn;
    int b = i % _sqrtn;

    // Remove from bottom (and top if bottom becomes empty)
    _bottom[a]->remove(b);
    if (_bottom[a]->_size == 0) {
        delete _bottom[a];
        _bottom[a] = nullptr;
        _top->remove(a);
        if(_top->_size == 0) {
            delete _top;
            _top = nullptr;
        }
    }

    // Update fields
    _size--;
    set_min_max();
}

int Tree::succ(int i) {
    return succ_aux(i+1);
}

// INT_MAX represents infinity (a successor does not exist)
int Tree::succ_aux(int i) {
    if (_size == 0 || i > _n - 1) return INT_MAX;
    if (_size == 1) return (_min >= i) ? _min : INT_MAX;

    int a = i / _sqrtn;
    int b = i % _sqrtn;

    int j = INT_MAX;
    if (_bottom[a] != nullptr && _bottom[a]->_max >= b) {
        j = a * _sqrtn + _bottom[a]->succ_aux(b);
    } else if (_top != nullptr) {
        int c = _top->succ_aux(a + 1);
        if (c != INT_MAX) {
            j = c * _sqrtn + _bottom[c]->_min;
        }
    }

    if (i <= _min && _min < j) {
        j = _min;
    }

    return j;
}

int Tree::find_min() {
    return _min;
}

void Tree::remove_min() {
    remove(find_min());
}

void Tree::pretty_print(int indent) {
    printf("Tree %d [min=%d max=%d size=%d]\n", _n, _min, _max, _size);
    if (_top != nullptr) {
        printf("%*s" "top ", indent + 4, " ");
        _top->pretty_print(indent + 4);
    }
    for (int i = 0; i < _sqrtn; i++) {
        if (_bottom[i] != nullptr) {
            printf("%*s" "bot[%d] ", indent + 4, " ", i);
            _bottom[i]->pretty_print(indent + 4);
        }
    }
}

void Tree::set_min_max() {
    if (_size == 0) {
        _min = _n;
        _max = -1;
        return;
    }

    if (_size == 1) {
        _max = _min;
        return;
    }

    _max = _top->_max * _sqrtn + _bottom[_top->_max]->_max;
}

