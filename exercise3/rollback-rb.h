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
            do_rollback(t);
            int result = tree.get(key)->key;
            undo_rollback(t);
            return result;
        }

        /**
         * Does a permanent rollback of the data structure, i.e.
         * reverts all changes until the given time.
         *
         * Should be used in pair with undo_rollback(int t) to
         * undo the revert of changes and bring the data structure
         * back into a consistent state!
         */
        void do_rollback(int t) {
            // Revert update at time t as well (slide 3)
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
        }

        /**
         * Reverts a permanent rollback by undoing the reverts
         * made by a call to do_rollback(int t).
         */
        void undo_rollback(int t) {
            // Perform update from time t as well (slide 3)
            for (int i = t; i < ops.size(); i++) {
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
        }

        // NOTE: THIS IMPLEMENTATION DOES NOT WORK! SEE BELOW!
        RBTree<int> rollback(int t) {
            // Simple optimization to ensure a constant of O(m/2)
            // ratehr than O(m), when rolling back further than
            // half-way into the past.
            // if (t < (ops.size() / 2)) {
            //     return rollforward(t);
            // }
            return rollbackward(t);
        }

        // NOTE: THIS IMPLEMENTATION DOES NOT WORK!
        // To do a rollback this way, we need a copy of the existing
        // data structure, which requires proper copy construction.
        // If copy construction takes time O(n log n), the running
        // time of a rollback-based query still takes O(m log n),
        // but with a significantly higher constant.
        RBTree<int> rollbackward(int t) {
            RBTree<int> result;
            for (int i = ops.size() - 1; i >= t; i--) {
                pair<Op,int> p = ops[i];

                switch (p.first) {
                    case INSERT:
                        result.remove(p.second);
                        break;
                    case DELETE:
                        result.insert(p.second, 0);
                        break;
                }
            }
            return result;
        }

        RBTree<int> rollforward(int t) {
            RBTree<int> result;
            for (int i = 0; i < t; i++) {
                pair<Op,int> p = ops[i];
                
                switch (p.first) {
                    case INSERT:
                        result.insert(p.second, 0);
                        break;
                    case DELETE:
                        result.remove(p.second);
                        break;
                }
            }
            return result;
        }
};