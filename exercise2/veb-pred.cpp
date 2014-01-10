#include <string>
#include <iostream>
#include "veb-heap.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    int pred = 1;
    if (argc > 1) {
        pred = atoi(argv[1]);
    }
    VebTree<string> *t = new VebTreeNode<string>(16);
    // t->insert(10, new VebNode<string>(10, "Alice"));
    // t->insert( 8, new VebNode<string>( 8, "Bobby"));
    // t->insert(12, new VebNode<string>(12, "Carlo"));
    t->insert( 0, new VebNode<string>( 0, "Doris"));

    VebNode<string> *p = t->predecessor(pred);
    cout << (p == NULL ? -1 : p->getKey()) << endl;
    return 0;
}