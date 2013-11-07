#include <stdlib.h>
#include <iostream>
#include "ITree.hpp"

class Tree : public ITree {

    private:

        ITree *top;
        ITree **bottom;

    public:

        Tree(int);
        ~Tree();

        int succ(int);
        void insert(int);
        void remove(int);

};

Tree::Tree(int n) : ITree(n) {
    this->top    = NULL;
    this->bottom = (ITree **) calloc(sqrtn, sizeof( ITree* ));
}

Tree::~Tree() {
    std::cout << "Destructing Tree(" << n << ")" << std::endl;
    delete top;
    for (int i = 0; i < sqrtn; i++) {
        delete bottom[i];
    }
    free(this->bottom);
}

// TODO size gets update even if the same index is inserted twice
void Tree::insert(int i) {
    int a = i / sqrtn;
    int b = i % sqrtn;

    // n = 1 is our stop condition
    if (n > 1) {
        // Lazy initialization of bottom
        if (bottom[a] == NULL) {
            std::cout << "  Create bottom[" << a << "] Tree(" << sqrtn << ")" << std::endl;
            bottom[a] = new Tree(sqrtn);
        }

        // Insert in top and bottom trees
        if (bottom[a]->getSize() == 0) {
            // Lazy initialization of top
            if (top == NULL) {
                std::cout << "  Create top Tree(" << sqrtn << ")" << std::endl;
                top = new Tree(sqrtn);
            }
            std::cout << "top.insert(" << a << ")" << std::endl;
            top->insert(a);
        }
        std::cout << "bottom["<<a<<"].insert(" << b << ")" << std::endl;
        bottom[a]->insert(b);
    }

    // Update fields
    size++;
    if (i < min) min = i;
    if (i > max) max = i;
}
