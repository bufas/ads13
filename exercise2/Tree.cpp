#include <cstdlib>
#include <iostream>
#include <limits.h>

#include "Tree.hpp"
#include "Leaf.hpp"

Tree::Tree(int n) : ITree(n), _top(nullptr), _bottom(new ITree*[_sqrtn]) {
    // Initialize top and bottom trees
    if (_sqrtn == 2)  {
        // Initialize as leaves
        _top = new Leaf();
        for (int i = 0; i < _sqrtn; i++) _bottom[i] = new Leaf();
    } else {
        // Initialize as trees
        _top = new Tree(_sqrtn);
        for (int i = 0; i < _sqrtn; i++) _bottom[i] = new Tree(_sqrtn);
    }
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
    if (_bottom[a]->get_size() == 0) {
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
        _min = _top->get_min() * _sqrtn + _bottom[_top->get_min()]->get_min();
        i = _min;
    }

    int a = i / _sqrtn;
    int b = i % _sqrtn;

    // Remove from bottom (and top if bottom becomes empty)
    _bottom[a]->remove(b);
    if (_bottom[a]->get_size() == 0) {
        _top->remove(a);
    }

    // Update fields
    _size--;
    set_min_max();
}

// INT_MAX represents infinity (a successor does not exist)
int Tree::succ(int i) {
    if (_size == 0) return INT_MAX;
    if (_size == 1) return (_min > i) ? _min : INT_MAX;

    int a = i / _sqrtn;
    int b = i % _sqrtn;

    int j = INT_MAX;
    if (_bottom[a]->get_max() >= b) {
        j = a * _sqrtn + _bottom[a]->succ(b);
    } else {
        int c = _top->succ(a + 1);
        if (c != INT_MAX) {
            j = c * _sqrtn + _bottom[c]->get_min();
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

    _max = _top->get_max() * _sqrtn + _bottom[_top->get_max()]->get_max();
}

