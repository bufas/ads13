#include "RBTree2.h"

int main(int argc, char *argv[]) {
    RBTree<bool> t = RBTree<bool>();
    int n = 5000;
    for (int i = 0; i < n; i++) {
        int key = n - i;
        t.insert(key,false);
    }
    //t.validate();
    return 0;
}