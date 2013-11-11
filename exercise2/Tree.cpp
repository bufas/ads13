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
    if (_bottom[a]->getSize() == 0) {
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
    // TODO
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
