#pragma once

#include "rb-tree/RBTree.h"

enum Op { INSERT, DELETE };

class Retro {

    public:
        virtual ~Retro() {};

        virtual void Insert(int t, Op o, int key) = 0;
        virtual void Delete(int t) = 0;
        virtual int Query(int x) = 0;

};