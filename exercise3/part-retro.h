#include <vector>
#include <utility>
#include <iostream>
#include "retro.h"
#include "rb-tree/RBTree.h"

using std::pair;
using std::vector;
using std::cout;
using std::endl;

class PartRetro : public Retro {
    
    public:

        vector<pair<Op,int>> timeline;
    	// RBTree<pair<Op,int>> timeline;
    	RBTree<int> tree; // The type parameter is never used

    	PartRetro() {}

        void Insert(Op o, int key) {
            Insert(timeline.size(), o, key);
        }

        void Insert(int t, Op o, int key) {
            // timeline.insert(t, make_pair(o, key));
        	timeline.insert(timeline.begin() + t, make_pair(o, key));
        	switch (o) {
        		case INSERT:
        			tree.insert(key, 0);
        			break;
        		case DELETE:
        			tree.remove(key);
        			break;
        	}
        }

        void Delete(int t) {
            // pair<Op,int> p = timeline.get(t)->value;
        	pair<Op,int> p = timeline[t];
        	switch (p.first) {
        		case INSERT:
        			tree.remove(p.second);
        			break;
        		case DELETE:
        			tree.insert(p.second, 0);
        			break;
        	}
            // timeline.remove(t);
        	timeline.erase(timeline.begin() + t);
        }

        int Query(int key) {
            return tree.predecessor(key);
        }

        int Query(int t, int key) {
            return -1;
        }

};