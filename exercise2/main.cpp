#include <iostream>
#include <cstdlib>
#include "heap.hpp"
#include "bin-heap.cpp"
#include "fib-heap.cpp"

using std::cout;
using std::endl;

int main(int argc, char **argv) {
    // Heap<int> *h = new BinHeap<int>;
    Heap<int> *h = new FibHeap<int>;
    cout << "Stuff happened" << endl;
    h->insert(4, 10);
    h->insert(2, 10);
    h->insert(5, 10);
    Node<int> *n1 = h->insert(7, 10);
    h->insert(1, 10);
    h->insert(2, 10);
    Node<int> *n2 = h->insert(8, 10);
    h->insert(3, 10);
    h->decreaseKey(n1, 3);
    h->decreaseKey(n2, 8);
    h->deleteMin();
    h->deleteMin();
    h->deleteMin();
    h->deleteMin();
    h->deleteMin();
    h->deleteMin();
    //h->deleteMin();
    //h->deleteMin();
    delete h;
    sleep(1);
    cout << "Stuff happened again" << endl;
    return 0;
}