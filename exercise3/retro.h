#pragma once

#include "RBTreeNode.h"

enum Op { INSERT, DELETE };

template<typename T>
class Retro {

    public:
        virtual ~Retro() {};

        virtual void Insert(int t, Op o, T value);
        virtual void Delete(int t);
        virtual RBTreeNode<T>* Query(int x);

};