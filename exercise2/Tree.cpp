#include <cstdlib>
#include <iostream>
#include <limits.h>

#include "Tree.hpp"
#include "Leaf.hpp"

Tree::Tree(int n) : ITree(n), _top(nullptr), _bottom(new ITree*[_sqrtn]) {
    std::cout << "Create Tree("<<_n<<") with sqrt = " << _sqrtn << std::endl;
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

    initialize_tree(&_bottom[a]);    // Lazy initialization of bottom

    // Insert in top and bottom trees
    if (_bottom[a]->get_size() == 0) {
        initialize_tree(&_top);      // Lazy initialization of top
        _top->insert(a);
    }
    _bottom[a]->insert(b);

    // Update fields
    _size++;
    if (i < _min) _min = i;
    if (i > _max) _max = i;
}

void Tree::remove(int i) {
    if (_size <= 1) {
        _size--;
        set_min_max();
        return;
    }

    if (_min == i) {
        int tmp = _top->get_min() * _sqrtn + _bottom[_top->get_min()]->get_min();
        i = _min = tmp;
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
    if (_size == 1) {
        // TODO this can be inserted in set_min_max, but needs i as parameter
        if (_min == i) _min = _max;
        if (_max == i) _max = _min;
    } else {
        set_min_max();
    }
}

// INT_MAX represents infinity (a successor does not exist)
int Tree::succ(int i) {
    if (_size == 0) return INT_MAX;

    int a = i / _sqrtn;
    int b = i % _sqrtn;

    int j = INT_MAX;
    if (_bottom[a] != nullptr && _bottom[a]->get_max() >= b) {
        j = a * _sqrtn + _bottom[a]->succ(b);
    } else if (_top != nullptr) {
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

void Tree::initialize_tree(ITree **tree) {
    if (*tree == nullptr) {
        if (_sqrtn == 2) {
            *tree = new Leaf();
        } else {
            *tree = new Tree(_sqrtn);
        }
    }
}

void Tree::pretty_print(int indent) {
    printf("%*s" "Tree %d [min=%d max=%d size=%d]\n", indent, " ", _n, _min, _max, _size);
    if (_top != nullptr) {
        printf("%*s" "top\n", indent + 4, " ");
        _top->pretty_print(indent + 4);
    }
    for (int i = 0; i < _sqrtn; i++) {
        if (_bottom[i] != nullptr) {
            printf("%*s" "bot[%d]\n", indent + 4, " ", i);
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

    _max = _top->get_max() * _sqrtn + _bottom[_top->get_max()]->get_max();
    _min = _top->get_min() * _sqrtn + _bottom[_top->get_min()]->get_min();
}

