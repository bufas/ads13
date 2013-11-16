#ifndef LEAF_HPP_
#define LEAF_HPP_

#include "ITree.hpp"

class Leaf : public ITree {

    private:

        bool entries[2];

    public:

        Leaf();
        virtual ~Leaf() override {}

        virtual int succ(int) override;
        virtual void insert(int) override;
        virtual void remove(int) override;
        virtual int find_min() override;
        virtual void remove_min() override;
        virtual void pretty_print(int) override;

};

#endif
