#pragma once

#include "rb-tree/RBTree.h"

enum Op { INSERT, DELETE };

class Retro {

    public:
        virtual ~Retro() {};

        virtual void Insert(int t, Op o, int key);
        virtual void Delete(int t);
        virtual RBTreeNode<int>* Query(int x);

};