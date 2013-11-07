#ifndef I_TREE_HPP_
#define I_TREE_HPP_

#include <math.h>

class ITree {

    protected:

        int size;
        int max;
        int min;
        int n;
        int sqrtn;

    public:

        ITree(int);

        int getSize();

        virtual int succ(int) = 0;
        virtual void insert(int) = 0;
        virtual void remove(int) = 0;

};

ITree::ITree(int n) {
    this->size  = 0;
    this->max   = -1;
    this->min   = n;
    this->n     = n;
    this->sqrtn = (int) sqrt(n); // TODO round to nearest power of 2
}

int ITree::getSize() {
    return size;
}

#endif
