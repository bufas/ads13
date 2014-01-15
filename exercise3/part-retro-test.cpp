#include <iostream>
#include <cassert>
#include "part-retro.h"


void test_normal_insert() {
    PartRetro t;
    t.Insert(INSERT, 1);
    t.Insert(INSERT, 3);
    t.Insert(INSERT, 2);
    t.Insert(INSERT, 4);
    assert(t.tree.size == 4 && "Size should be 4");
    t.Insert(DELETE, 4);
    t.Insert(DELETE, 1);
    t.Insert(DELETE, 3);
    assert(t.tree.size == 1 && "Size should be 1");
}

void test_retro_insert() {
    PartRetro t;
    
    // Make a few normal inserts first
    t.Insert(INSERT, 1);
    t.Insert(INSERT, 5);
    t.Insert(INSERT, 2);

    // Insert in the past
    t.Insert(1, INSERT, 9);
    t.Insert(0, INSERT, 7);
    t.Insert(2, INSERT, 3);
    assert(t.tree.size == 6 && "Size should be 6");

    // Some more present inserts
    t.Insert(INSERT, 10);
    t.Insert(INSERT, 11);
    t.Insert(INSERT, 12);
    assert(t.tree.size == 9 && "Size should be 9");

    // Insert some deletes in the past
    t.Insert(3, DELETE, 1);
    t.Insert(3, DELETE, 2);
    t.Insert(3, DELETE, 5);
    assert(t.tree.size == 6 && "Size should be 6 again");
}

int main(int argc, char *argv[]) {
    test_normal_insert();
    test_retro_insert();
    std::cout << "Done!" << std::endl;
    return 0;
}