#pragma once

#include "retro.h"

template<typename T>
class FullRetro : public Retro<T> {
    
    public:

        void Insert(int t, Op o, T value) {

        }
        void Delete(int t) {

        }

        RBTreeNode<T>* Query(int x) {
            return nullptr;
        }

        RBTreeNode<T>* Query(int t, int x) {
            return nullptr;
        }

};