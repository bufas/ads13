#include <iostream>
#include <unistd.h>
#include <cmath>
#include "Tree.hpp"

int main() {
    Tree t(16);

    for (int i = 0; i < 16; i++) {
        t.insert(i);
    }

    printf("Done inserting");

    printf("\n");
    t.pretty_print(0);
    printf("\n");

    t.remove(13);

    for (int i = 0; i < 16; i++) {
        printf("Successor of %d is %d\n", i, t.succ(i));
    }

    return 0;
}

