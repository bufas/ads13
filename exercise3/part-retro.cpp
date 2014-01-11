#pragma once

#include "retro.h"

template<typename T>
class PartRetro : public Retro<T> {
    
    public:

        void Insert(int t, Op o, T value) {

        }
        void Delete(int t) {

        }

        RBTreeNode<T>* Query(int x) {
            return nullptr;
        }

};