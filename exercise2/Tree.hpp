#ifndef TREE_HPP_
#define TREE_HPP_

class Tree {

    private:

        int _size;
        int _max;
        int _min;
        int _n;
        int _sqrtn;

        Tree *_top;
        Tree **_bottom;

        void set_min_max();
        virtual int succ_aux(int);

    public:

        Tree(int);
        virtual ~Tree();

        virtual int succ(int);
        virtual void insert(int);
        virtual void remove(int);
        virtual int find_min();
        virtual void remove_min();
        virtual void pretty_print(int);

};

#endif
