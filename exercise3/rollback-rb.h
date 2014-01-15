#pragma once

#include <vector>
#include <utility>
#include "retro.h"
#include "rb-tree/RBTree.h"

using std::pair;
using std::make_pair;
using std::vector;

class RollbackRBTree : public Retro {
    
    public:

        RBTree<int> tree;
        vector<pair<Op,int>> ops;

        RollbackRBTree() {}

        ~RollbackRBTree() {
            ops.clear();
        }

        void Insert(Op o, int key) {
            Insert(ops.size(), o, key);
        }

        void Insert(int t, Op o, int key) {
            switch (o) {
                case INSERT:
                    tree.insert(key, 0);
                    break;
                case DELETE:
                    tree.remove(key);
                    break;
            }
            ops.insert(ops.begin() + t, make_pair(o, key));
        }

        void Delete(int t) {
            pair<Op,int> p = ops[t];
            switch (p.first) {
                case INSERT:
                    tree.remove(p.second);
                    break;
                case DELETE:
                    tree.insert(p.second, 0);
                    break;
            }
            ops.erase(ops.begin() + t);
        }

        int Query(int key) {
            return tree.get(key)->key;
        }

        int Query(int t, int key) {
            return rollback(t).get(key)->key;
        }

        RBTree<int> rollback(int t) {
            // Simple optimization to ensure a constant of O(m/2)
            // ratehr than O(m), when rolling back further than
            // half-way into the past.
            // if (t < (ops.size() / 2)) {
            //     return rollforward(t);
            // }
            return rollbackward(t);
        }

        RBTree<int> rollbackward(int t) {
            RBTree<int> tree;
            for (int i = ops.size() - 1; i >= t; i--) {
                pair<Op,int> p = ops[i];

                switch (p.first) {
                    case INSERT:
                        tree.remove(p.second);
                        break;
                    case DELETE:
                        tree.insert(p.second, 0);
                        break;
                }
            }
            return tree;
        }

        RBTree<int> rollforward(int t) {
            RBTree<int> tree;
            for (int i = 0; i < t; i++) {
                pair<Op,int> p = ops[i];
                
                switch (p.first) {
                    case INSERT:
                        tree.insert(p.second, 0);
                        break;
                    case DELETE:
                        tree.remove(p.second);
                        break;
                }
            }
            return tree;
        }
};