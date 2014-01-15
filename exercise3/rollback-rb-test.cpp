#include "rollback-rb.h"

int main(int argc, char *argv[]) {
    RollbackRBTree t;
    t.Insert(INSERT, 1);  // 1
    t.Insert(INSERT, 2);  // 1 2
    t.Insert(INSERT, 3);  // 1 2 3
    t.Insert(DELETE, 3);  // 1 2
    t.Insert(INSERT, 4);  // 1 2 4
    t.Insert(DELETE, 2);  // 1 4
    t.tree.print();

    RBTree<int> rbt;
    rbt.insert(1, 0);
    rbt.insert(2, 0);
    rbt.insert(1, 0);
    rbt.insert(1, 0);
    rbt.insert(1, 0);
    rbt.insert(1, 0);
    rbt.print();
    // rbt.insert(1, 0);
    // rbt.insert(4, 0);
    // rbt.print();
    return 0;
}
