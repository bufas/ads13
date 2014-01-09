#ifndef NODE_HPP_
#define NODE_HPP_

template<typename T>
class Node {
    public:
        virtual int getKey() = 0;
        virtual T getValue() = 0;
};

#endif