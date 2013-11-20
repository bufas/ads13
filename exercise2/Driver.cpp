#include "Tree.hpp"

#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <cmath>

#define TREESIZE 64

int main() {
    Tree t(TREESIZE);

    if (t.succ(0) != INT_MAX) printf("ERROR! 1\n");
    else printf("SUCCESS 1\n");

    t.insert(4);
    if (t.succ(0) != 4) printf("ERROR! 2\n");
    else printf("SUCCESS 2\n");
    if (t.succ(5) != INT_MAX) printf("ERROR! 3\n");
    else printf("SUCCESS 3\n");

    t.remove(4);
    if (t.succ(0) != INT_MAX) printf("ERROR! 4\n");
    else printf("SUCCESS 4\n");

    for (int i = 0; i < TREESIZE; i++) t.insert(i);
    for (int i = 0; i < TREESIZE - 1; i++) {
        if (t.succ(i) != i + 1) printf("ERROR! 5 succ(%d) != %d it is %d\n", i, i + 1, t.succ(i));
        else printf("SUCCESS 5\n");
    }

    for (int i = 1; i < TREESIZE; i++) {
        if (t.pred(i) != i - 1) printf("ERROR! 6 pred(%d) != %d it is %d\n", i, i - 1, t.pred(i));
        else printf("SUCCESS 6\n");
    }

    // LOL multiple inserts

    Tree t2(TREESIZE);

    t2.insert(1);
    t2.insert(1);
    t2.insert(1);

    t2.remove(1);

    t2.insert(12);
    t2.insert(12);

    t2.decrease_key(12, 6);

    t2.pretty_print(0);



    return 0;
}

