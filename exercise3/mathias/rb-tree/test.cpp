#include <iostream>
#include <string>
#include <stdexcept>

#include "RBTree.h"

using std::cout;
using std::endl;
using std::string;

void ex(bool b, string s) {
    if (b) {
        cout << s << endl;
        throw std::runtime_error(s);
    }
}

void is_null(void *p, string s) {
    ex(p != nullptr, s);
}

void not_null(void *p, string s) {
    ex(p == nullptr, s);
}

void is_equal(int actual, int expected) {
    string s = std::string("expected ");
    s += std::to_string(expected);
    s += std::string("; got ");
    s += std::to_string(actual);
    ex(actual != expected, s);
}

void test_get() {
    RBTree<int> t;
    t.insert(1,10);
    t.insert(2,10);
    t.insert(3,10);
    t.insert(4,10);
    not_null(t.get(4), "get(4) should not be null");
    not_null(t.get(2), "get(2) should not be null");
    not_null(t.get(1), "get(1) should not be null");
    not_null(t.get(3), "get(3) should not be null");

    t.insert(5,10);
    not_null(t.get(5), "get(5) should not be null");
    is_null(t.get(10), "get(10) should be null");
    is_null(t.get(0),  "get(0) should be null");
}

void test_insert() {
    RBTree<int> t;
    t.insert(2,10);
    is_equal(t.size, 1);
    t.insert(1,10);
    is_equal(t.size, 2);
    is_equal(t.find_min()->key, 1);
    t.remove(1);
    is_equal(t.find_min()->key, 2);
}

void test_inorder_aux(RBTreeNode<int> *n) {
    switch (n->key) {
        case  5: cout << "a"; break;
        case 10: cout << "b"; break;
        case 20: cout << "c"; break;
        default: cout << "FUCK YOU";
    }
    cout << ":" << n->value << " ";
}

void test_inorder() {
    // The times at which we want to insert duplicates
    int a = 5, b = 10, c = 20;

    RBTree<int> t;
    t.insert(b, 1);
    t.insert(b, 2);
    t.insert(b, 3);
    t.insert(b, 4);
    t.insert(b, 5);
    t.insert(b, 6);

    // Break the cycle
    t.insert(a, 1);
    t.insert(a, 2);

    t.insert(b, 7);
    t.insert(b, 8);
    t.insert(b, 9);

    // Break it again
    t.insert(c, 1);
    t.insert(c, 2);
    t.insert(c, 3);

    // Interleaved inserts
    t.insert(b, 10);
    t.insert(a, 3);
    t.insert(b, 11);
    t.insert(c, 4);
    t.insert(c, 5);
    t.insert(a, 4);
    t.insert(c, 6);
    t.insert(b, 12);
    t.insert(b, 13);

    t.inorder(test_inorder_aux);
    cout << endl;
    t.print();
}

void test_inorder_no_value() {
    RBTree<int> t;

    t.print();
}

int main(int argc, char *argv[]) {
    test_get();
    test_insert();
    test_inorder();
    test_inorder_no_value();

    RBTree<int> t = RBTree<int>();
    int n = 5000;
    for (int i = 0; i < n; i++) {
        int key = n - i;
        t.insert(key, i);
    }
    t.validate();
    return 0;
}