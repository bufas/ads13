#include <utility>
#include "retro.h"
#include "rb-tree/RBTree.h"

using std::pair;

class PartRetro : public Retro {
    
    public:

    	RBTree<pair<Op, int>> *timeline;
    	RBTree<int> *tree; // The type parameter is never used

    	PartRetro() 
    		: timeline(new RBTree<pair<Op, int>>), tree(new RBTree<int>) {}

        void Insert(Op o, int key) {
            Insert(tree->size(), o, key);
        }

        void Insert(int t, Op o, int key) {
        	timeline->insert(t, new pair<Op, int>(o, key));

        	switch (o) {
        		case INSERT:
        			tree->insert(key, nullptr);
        			break;
        		case DELETE:
        			tree->remove(key);
        			break;
        	}
        }

        void Delete(int t) {
        	pair<Op, int> *p = timeline->get(t)->value;

        	switch (p->first) {
        		case INSERT:
        			tree->remove(p->second);
        			break;
        		case DELETE:
        			tree->insert(p->second, nullptr);
        			break;
        	}

        	timeline->remove(t);
        }

        int Query(int key) {
            return Query(0, key);
        }

        int Query(int t, int key) {
            return tree->predecessor(key);
        }

};