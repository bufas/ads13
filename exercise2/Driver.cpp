#include <iostream>
#include <cmath>
#include "Tree.hpp"

int main() {
    Tree t(6);

    t.insert(4);
    t.insert(1);

    std::cout << "Successor of 1 is " << t.succ(1) << std::endl;
    std::cout << "Successor of 2 is " << t.succ(2) << std::endl;
    std::cout << "Successor of 5 is " << t.succ(5) << std::endl;

    return 0;
}

