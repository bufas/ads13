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

    //t.pretty_print(0);

    for (int i = 1; i < TREESIZE; i++) {
        if (t.pred(i) != i - 1) printf("ERROR! 6 pred(%d) != %d it is %d\n", i, i - 1, t.pred(i));
        else printf("SUCCESS 6\n");
    }


    // for (int i = 0; i < 4; i++) {
    //     printf("Inserting %d\n", i);
    //     t.insert(i);
    //     t.pretty_print(0);
    // }

    // printf("\n");
    // printf("Remove 2\n");
    // t.remove(2);

    // t.pretty_print(0);
    // printf("\n");


    // for (int i = 0; i < 4; i++) {
    //     printf("Successor of %d is %d\n", i, t.succ(i));
    // }

    return 0;
}

