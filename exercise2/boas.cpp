#include <cmath>

class Tree;
class Tree {

    private:

        int size;
        int max;
        int min;
        int n;

        Tree *top;
        Tree *bottom[];

    public:

        Tree(int);
        ~Tree();

        int findSucc(int);
        int insert(int);
        int remove(int);

};

Tree::Tree(int n) {
    this->size = 0;
    this->max = -1;
    this->min = n;
    this->n = n;

    int sqrtn = (int) std::sqrt( n );
    this->bottom = malloc( sqrtn * sizeof(Tree *));
}


