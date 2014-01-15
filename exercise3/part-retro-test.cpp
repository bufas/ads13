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

void test_retro_delete() {
    PartRetro t;
    for (int i = 0; i < 500; i++) {
        t.Insert(INSERT, i);
    }
    t.Insert(INSERT, 5);
    t.Insert(INSERT, 6);
    t.Insert(INSERT, 7);
    assert(t.tree.size == 503 && "Size should be 503");
    for (int i = 0; i < 500; i++) {
        t.Delete(0);
    }
    assert(t.tree.size == 3 && "Size should now be 3");
}

void test_retro_delete_again() {
    PartRetro t;
    // Insert [1..500]
    for (int i = 0; i < 500; i++) {
        t.Insert(INSERT, i + 1);
    }
    // Delete them again
    for (int i = 0; i < 500; i++) {
        t.Insert(DELETE, 500 - i);
    }
    assert(t.tree.size == 0 && "Size should be 0");
    // assert(t.timeline.size == 1000 && "Should be 1000 operations");
    assert(t.timeline.size() == 1000 && "Should be 1000 operations");

    // Then delete all the deletes
    for (int i = 500; i < 1000; i++) {
        t.Delete(500);
    }
    assert(t.tree.size == 500 && "Size should now be 500");
    // assert(t.timeline.size == 500 && "Should be 500 operations");
    assert(t.timeline.size() == 500 && "Should be 500 operations");

    // Then delete all the inserts
    for (int i = 0; i < 500; i++) {
        t.Delete(0);
    }
    assert(t.timeline.size() == 0 && "Should be 0 operations");
    assert(t.tree.size == 0 && "Should be 0 elements");
}

int main(int argc, char *argv[]) {
    test_normal_insert();
    test_retro_insert();
    test_retro_delete();
    test_retro_delete_again();
    std::cout << "Done!" << std::endl;
    return 0;
}