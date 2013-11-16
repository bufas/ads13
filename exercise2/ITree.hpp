#ifndef I_TREE_HPP_
#define I_TREE_HPP_

class ITree {

    protected:

        int _size;
        int _max;
        int _min;
        int _n;
        int _sqrtn;

    public:

        ITree(int);
        virtual ~ITree() {}

        int get_size();
        int get_max();
        int get_min();

        virtual int succ(int) = 0;
        virtual void insert(int) = 0;
        virtual void remove(int) = 0;
        virtual int find_min() = 0;
        virtual void remove_min() = 0;
        virtual void pretty_print(int) = 0;

};

#endif
