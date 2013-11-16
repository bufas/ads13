#include <iostream>
#include <unistd.h>
#include <cmath>
#include "Tree.hpp"

int main() {
    Tree t(6);

    t.insert(3);
    t.insert(2);
    t.pretty_print(0); printf("\n");

    printf("Min = %d\n", t.find_min());
    t.remove_min();
    printf("Min = %d\n", t.find_min());

    t.pretty_print(0); printf("\n");

    std::cout << "Successor of 1 is " << t.succ(1) << std::endl;
    std::cout << "Successor of 2 is " << t.succ(2) << std::endl;
    std::cout << "Successor of 5 is " << t.succ(5) << std::endl;

    return 0;
}

