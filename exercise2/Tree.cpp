#include <cstdlib>
#include <iostream>

#include "Tree.hpp"
#include "Leaf.hpp"

Tree::Tree(int n) : ITree(n), _top(nullptr), _bottom(new ITree*[_sqrtn]) {
    std::cout << "Create Tree("<<_n<<") with sqrt = " << _sqrtn << std::endl;
    std::fill(_bottom, _bottom + _sqrtn, nullptr);
}

Tree::~Tree() {
    std::cout << "Destructing Tree(" << _n << ")" << std::endl;
    delete _top;
    for (int i = 0; i < _sqrtn; i++) {
        delete _bottom[i];
    }
    delete[] _bottom;
}

// TODO size gets update even if the same index is inserted twice
void Tree::insert(int i) {
    std::cout << "INSERTING " << i << std::endl;
    int a = i / _sqrtn;
    int b = i % _sqrtn;

    initialize_tree(&_bottom[a]);    // Lazy initialization of bottom

    // Insert in top and bottom trees
    if (_bottom[a]->get_size() == 0) {
        initialize_tree(&_top);      // Lazy initialization of top
        std::cout << "top.insert(" << a << ")" << std::endl;
        _top->insert(a);
    }
    std::cout << "bottom["<<a<<"].insert("<<b<<")" << std::endl;
    _bottom[a]->insert(b);

    // Update fields
    _size++;
    if (i < _min) _min = i;
    if (i > _max) _max = i;
}

void Tree::remove(int i) {
    std::cout << "REMOVING " << i << std::endl;
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

int Tree::succ(int i) {
    return -1; // TODO
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

void Tree::set_min_max() {
    // TODO verify the correctness of this
    if (_size == 0) {
        _min = _n;
        _max = -1;
        return;
    }

    _max = _top->get_max() * _sqrtn + _bottom[_top->get_max()]->get_max();
    _min = _top->get_min() * _sqrtn + _bottom[_top->get_min()]->get_min();
}

