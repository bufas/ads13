#ifndef TREE_HPP_
#define TREE_HPP_

#include "ITree.hpp"

class Tree : public ITree {

    private:

        ITree *_top;
        ITree **_bottom;

        void initialize_tree(ITree **);
        void set_min_max();

    public:

        Tree(int);
        virtual ~Tree() override;

        virtual int succ(int) override;
        virtual void insert(int) override;
        virtual void remove(int) override;

};

#endif
