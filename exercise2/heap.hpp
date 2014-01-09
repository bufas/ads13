#ifndef HEAP_HPP_
#define HEAP_HPP_

#include "node.hpp"

template<typename T>
class Heap {
    public:
        virtual ~Heap() {};

        virtual Node<T>* insert(int key, T value) = 0;
        virtual Node<T>* findMin() = 0;
        virtual Node<T>* deleteMin() = 0;
        virtual void decreaseKey(Node<T> *n, int delta) = 0;
        virtual int size() = 0;
};

#endif