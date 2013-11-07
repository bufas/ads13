#include "ITree.hpp"

class Leaf : ITree {

    private:

        bool entries[2];

    public:

        Leaf() : ITree(2) {}

};

