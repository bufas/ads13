#include <iostream>
#include <unistd.h>
#include <cmath>
#include "Tree.hpp"

int main() {
    Tree t(16);

    for (int i = 0; i < 16; i++) {
        printf("Inserting %d\n", i);
        t.insert(i);
        t.pretty_print(0);
    }

    printf("\n");
    printf("Remove 2\n");
    t.remove(2);

    t.pretty_print(0);
    printf("\n");


    for (int i = 0; i < 16; i++) {
        printf("Successor of %d is %d\n", i, t.succ(i));
    }

    return 0;
}

