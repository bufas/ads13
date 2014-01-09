#include <string>
#include <iostream>
#include "veb-heap.cpp"

using namespace std;

int main() {
    VebHeap<std::string> h(8);
    h.insert(2, "Alice");
    cout << "size: " << h.size() << endl;
    h.insert(5, "Bob");
    cout << "size: " << h.size() << endl;
    h.insert(9, "Carl");
    cout << "size: " << h.size() << endl;
    h.insert(3, "Doris");
    cout << "size: " << h.size() << endl;
    Node<std::string> *n = h.insert(9, "Eunice");
    cout << "size: " << h.size() << endl;
    h.deleteMin();
    cout << "size: " << h.size() << endl;
    h.decreaseKey(n, 8);
    cout << "size: " << h.size() << endl;
    h.deleteMin();
    cout << "size: " << h.size() << endl;
    h.deleteMin();
    cout << "size: " << h.size() << endl;
    h.deleteMin();
    cout << "size: " << h.size() << endl;
    h.deleteMin();
    cout << "size: " << h.size() << endl;
    return 0;
}