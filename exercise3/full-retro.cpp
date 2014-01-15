#include "retro.h"
#include "rb-tree/RBTree.h"

class FullRetro : public Retro {
    
    public:

        Tree timeline;
        RBTree<int> tree;
        RBTree<int> inserts;

        void Insert(int t, Op o, int key) {
            // Insert into timeline at time t
            timeline.insert(t, o, key);
        }
        
        void Delete(int t) {

        }

        RBTreeNode<int>* Query(int x) {
            return nullptr;
        }

        RBTreeNode<int>* Query(int t, int x) {
            return nullptr;
        }

};