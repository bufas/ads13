#include "retro.h"
#include "rb-tree/RBTree.h"

class FullRetro : public Retro {
    
    public:

        RBTree<RBTree<int>*> st;

        FullRetro() {}

        void Insert(Op o, int key) {
            Insert(st.size, o, key);
        }

        void Insert(int t, Op o, int key) {

        }
        
        void Delete(int t) {

        }

        int Query(int key) {
            return Query(st.size, key);
        }

        int Query(int t, int key) {
            return -1;
        }

};