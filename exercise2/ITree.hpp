#ifndef I_TREE_HPP_
#define I_TREE_HPP_

#include <math.h>

class ITree {

    protected:

        int _size;
        int _max;
        int _min;
        int _n;
        int _sqrtn;

    public:

        ITree(int);
        virtual ~ITree() {}

        int getSize();

        virtual int succ(int) = 0;
        virtual void insert(int) = 0;
        virtual void remove(int) = 0;

};

#endif
