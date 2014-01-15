#include "RBTree.h"

int main(int argc, char *argv[]) {
    RBTree<int> t = RBTree<int>();
    int n = 5000;
    for (int i = 0; i < n; i++) {
        int key = n - i;
        t.insert(key, i);
    }
    t.validate();
    return 0;
}