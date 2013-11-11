#include <iostream>
#include <cmath>
#include "Tree.hpp"

int main() {
    Tree t(6);
    t.insert(4);
    t.insert(1);
    t.remove(4);
    t.insert(4);
    t.remove(1);
    t.remove(4);
    return 0;
}

