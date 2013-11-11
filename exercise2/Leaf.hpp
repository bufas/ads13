#ifndef LEAF_HPP_
#define LEAF_HPP_

#include "ITree.hpp"

class Leaf : public ITree {

    private:

        bool entries[2];

    public:

        Leaf();
        virtual ~Leaf() override {}

        virtual void insert(int) override;
        virtual void remove(int) override;
        virtual int succ(int) override;

};

#endif
